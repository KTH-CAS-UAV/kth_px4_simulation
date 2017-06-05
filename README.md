# kth px4 simulation

repo for KTH CAS UAV px4 simulation files.

Contains information and nodes to install and run a the PX4 basaed SitL-gazebo simulation with the Iris drone.

## instructions 
todo:
- [] installing ros indigo + gazebo 7 with mavros, mavlink and SitL-gazebo (px4 addapted rotor simulator)
- [] instruction on how to add difrent sensors to drone model (lidar, RGB(D)-cameras etc.)
- [x] instructions on how to build a simple world in gazebo
- [] document with frequent problems + answers (reset of model problem, catkin make to catkin build etc.)
- [] instructions how to run it using multiple computers (Gazebo on gpu) and rest on nuc for example

## gazebo-assets 
todo:
- [] include the simple world files ([x] cage simulation, Daniels maze, etc)
- [] include models of custemized drones ([]with lidar, [x] d-camera etc)

## ros-nodes 
todo:
- [] teleop node for joystick control
- [x] offboard node for rviz control
- [] launch examples:  
-- [x] example with octomap and custem world  
-- [] ???
- [x] simple urdf drone model for rviz visu ( [] if more time and more Kevin we can try using mashes and stuff)
- [] include iris drone with smaller hitbox for collision avoidence.


