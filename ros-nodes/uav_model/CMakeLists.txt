cmake_minimum_required(VERSION 2.8.3)
project(uav_model)

## Find catkin macros and libraries
find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  tf
)


################################################
## Declare ROS dynamic reconfigure parameters ##
################################################



###################################
## catkin specific configuration ##
###################################
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES mrmodel
#  CATKIN_DEPENDS urdf
#  DEPENDS system_lib
)

###########
## Build ##
###########

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(uav_tf src/uav_tf.cpp)
target_link_libraries(uav_tf ${catkin_LIBRARIES})

#############
## Install ##
#############

#############
## Testing ##
#############

