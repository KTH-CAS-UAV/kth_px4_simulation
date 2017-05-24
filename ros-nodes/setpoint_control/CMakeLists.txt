cmake_minimum_required(VERSION 2.8.3)
project(setpoint_control)


find_package(catkin REQUIRED COMPONENTS
  mavros_msgs
  roscpp
  std_msgs
  dynamic_reconfigure
)


################################################
## Declare ROS dynamic reconfigure parameters ##
################################################
generate_dynamic_reconfigure_options(
  cfg/uav_altitude.cfg
)


###################################
## catkin specific configuration ##
###################################
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES takeofftest
#  CATKIN_DEPENDS mavros_msgs roscpp std_msgs
#  DEPENDS system_lib
)

###########
## Build ##
###########

include_directories(
  ${catkin_INCLUDE_DIRS}
)


## Declare a C++ executable
add_executable(setpoint_control src/setpoint.cpp)

## Add cmake target dependencies of the executable
## same as for the library above
add_dependencies(setpoint_control ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

add_dependencies(setpoint_control ${PROJECT_NAME}_gencfg)

## Specify libraries to link a library or executable target against
target_link_libraries(setpoint_control
  ${catkin_LIBRARIES}
)


#############
## Install ##
#############



#############
## Testing ##
#############

