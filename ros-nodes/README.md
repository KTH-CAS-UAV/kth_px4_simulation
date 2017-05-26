# ROS-nodes for kth px4 simulation

Ros-nodes to control the UAV in the simulation as well as a simple urdf modle (currently only box model)

## uav_model 
contains a simple urdf modle for arviz as well as an example launch file to run simulation use:
```bash
roslaunch uav_model SitL_default.launch
```

This starts gazebo with the cage world rviz as well as the simple setpoint control node.

## setpoint_control
simple setpoint control node who listens to the topic "/nav_2D" and sends it as new goal to the UAV.  
Can be used to send the drone view the navgoal tool in rviz.  
The altitude can be controlled using rqt_reconfigure:

```bash
 rosrun rqt_reconfigure rqt_reconfigure
```



