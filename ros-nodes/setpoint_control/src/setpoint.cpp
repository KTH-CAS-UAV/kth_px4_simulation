// simple offboard control node for px4 with setpoints

#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>

// Dynamic reconfigure includes.
#include <dynamic_reconfigure/server.h>
// Auto-generated from cfg/ directory.
#include <setpoint_control/uav_altitudeConfig.h>

mavros_msgs::State current_state;
void state_cb(const mavros_msgs::State::ConstPtr& msg){
    current_state = *msg;
}
geometry_msgs::PoseStamped pose;
void nav_goal_callback(const geometry_msgs::PoseStamped::ConstPtr& msg)
{
    pose.pose.position.x = msg->pose.position.x;
    pose.pose.position.y = msg->pose.position.y;

    pose.pose.orientation.z = msg->pose.orientation.z;
    pose.pose.orientation.w = msg->pose.orientation.w;
    ROS_INFO_STREAM("New Target recived");  

    }

double uav_altitude;

void configCallback(setpoint_control::uav_altitudeConfig &config, uint32_t level)
{
     ROS_INFO("Reconfigure");
     uav_altitude=config.uav_altitude;
    
} // end configCallback()

geometry_msgs::PoseStamped targetPoint;

void targetCallback(const geometry_msgs::PoseStamped::ConstPtr& msg)
    {
      targetPoint.pose=msg->pose;
      targetPoint.header.frame_id="world";
      //ROS_INFO_STREAM("targetPointCallback");

    }


int main(int argc, char **argv)
{
    ros::init(argc, argv, "setpoint_control");
    ros::NodeHandle nh;
    ROS_INFO("Starting");
    ros::Subscriber state_sub = nh.subscribe<mavros_msgs::State>
            ("mavros/state", 10, state_cb);
    ros::Subscriber sub_nav_goal = nh.subscribe("/nav_2D",  1000, nav_goal_callback);
    
    ros::Subscriber sub_target = nh.subscribe("/mavros/setpoint_position/local", 1, &targetCallback);
        
    ros::Publisher local_pos_pub = nh.advertise<geometry_msgs::PoseStamped>
            ("mavros/setpoint_position/local", 10);

    ros::Publisher pub_target = nh.advertise<geometry_msgs::PoseStamped>("/target", 1);

    ros::ServiceClient arming_client = nh.serviceClient<mavros_msgs::CommandBool>
            ("mavros/cmd/arming");
    ros::ServiceClient set_mode_client = nh.serviceClient<mavros_msgs::SetMode>
            ("mavros/set_mode");

    //the setpoint publishing rate MUST be faster than 2Hz
    ros::Rate rate(20.0);
    
    uav_altitude=1.5;
    // Set up a dynamic reconfigure server.
    // This should be done before reading parameter server values.
    dynamic_reconfigure::Server<setpoint_control::uav_altitudeConfig> dr_srv;
    dynamic_reconfigure::Server<setpoint_control::uav_altitudeConfig>::CallbackType cb;
    cb = boost::bind(configCallback, _1, _2);
    dr_srv.setCallback(cb);

    // Initialize node parameters from launch file or command line.
    // Use a private node handle so that multiple instances of the node can
    // be run simultaneously while using different parameters.
    // Parameters defined in the .cfg file do not need to be initialized here
    // as the dynamic_reconfigure::Server does this for you.
    ros::NodeHandle private_node_handle_("~");
    private_node_handle_.param("uav_altitude", uav_altitude, double(1.5));
    
    // wait for FCU connection
    while(ros::ok() && current_state.connected){
        ros::spinOnce();
        rate.sleep();
    }
    ROS_INFO("FCU connected");

    //geometry_msgs::PoseStamped pose;
    pose.pose.position.x = 0;
    pose.pose.position.y = 0;
    pose.pose.position.z = uav_altitude;
    pose.pose.orientation.x = 0;
    pose.pose.orientation.y = 0;
    pose.pose.orientation.z = 0;
    pose.pose.orientation.w = 1.0;

    //send a few setpoints before starting
    for(int i = 100; ros::ok() && i > 0; --i){
        local_pos_pub.publish(pose);
        ros::spinOnce();
        rate.sleep();
    }

    mavros_msgs::SetMode offb_set_mode;
    offb_set_mode.request.custom_mode = "OFFBOARD";

    mavros_msgs::CommandBool arm_cmd;
    arm_cmd.request.value = true;

    ros::Time last_request = ros::Time::now();
    ROS_INFO("entering while");
    while(ros::ok()){
        pose.pose.position.z = uav_altitude;
        if( current_state.mode != "OFFBOARD" &&
            (ros::Time::now() - last_request > ros::Duration(1.0))){
            if( set_mode_client.call(offb_set_mode) &&
                offb_set_mode.response.success){
                ROS_INFO("Offboard enabled");
            }
            last_request = ros::Time::now();
        } else {
            if( !current_state.armed &&
                (ros::Time::now() - last_request > ros::Duration(1.0))){
                if( arming_client.call(arm_cmd) &&
                    arm_cmd.response.success){
                    ROS_INFO("Vehicle armed");
                }
                last_request = ros::Time::now();
            }
        }

        local_pos_pub.publish(pose);

        //publish target as pose
        pub_target.publish(targetPoint);

        ros::spinOnce();
        rate.sleep();
    }

    return 0;
}