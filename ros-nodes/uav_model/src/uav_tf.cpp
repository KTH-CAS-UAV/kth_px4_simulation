#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <geometry_msgs/PoseStamped.h>
#include "gazebo_msgs/ModelStates.h"

//simple tf publishe from the gazebo simulation to the uav_robot

class uav_tf
{
  public:
    uav_tf()
    {
      sub = node.subscribe("/mavros/local_position/pose", 10, &uav_tf::mavrosposeCallback, this);
    }

  private:
    void mavrosposeCallback(const geometry_msgs::PoseStamped::ConstPtr& msg){
    
    tf::Transform transform;
    
    transform.setOrigin( tf::Vector3(msg->pose.position.x, msg->pose.position.y, msg->pose.position.z) );
    tf::Quaternion q(msg->pose.orientation.x, msg->pose.orientation.y, msg->pose.orientation.z, msg->pose.orientation.w);
    transform.setRotation(q);
    br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "world", "drone_base"));
  }

  tf::TransformBroadcaster br;
  ros::NodeHandle node;
  ros::Subscriber sub;
  ros::Subscriber subgazebo;
  ros::Publisher pubgazebo;

};

int main(int argc, char **argv){
  ros::init(argc, argv, "uav_tf_pub");
  uav_tf tp;  

  ros::spin();
  
  return 0;
};