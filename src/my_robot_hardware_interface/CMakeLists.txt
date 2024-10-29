cmake_minimum_required(VERSION 3.8)
project(my_robot_hardware_interface)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(hardware_interface REQUIRED)
find_package(pluginlib REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(canopen REQUIRED)

# Include directories
include_directories(include)

# Add source files
add_library(${PROJECT_NAME} SHARED
  src/my_robot_hardware_interface.cpp
)

target_include_directories(${PROJECT_NAME}
  PRIVATE
    include
)

ament_target_dependencies(${PROJECT_NAME}
  rclcpp
  hardware_interface
  pluginlib
  rclcpp_lifecycle
  std_msgs
  geometry_msgs
  sensor_msgs
  canopen
)

# Register the hardware interface as a plugin
pluginlib_export_plugin_description_file(hardware_interface my_robot_hardware_interface.xml)

# Install targets
install(
  DIRECTORY include/
  DESTINATION include
)

install(
  TARGETS ${PROJECT_NAME}
  EXPORT export_${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

ament_export_include_directories(include)
ament_export_libraries(${PROJECT_NAME})
ament_export_dependencies(
  rclcpp
  hardware_interface
  pluginlib
  rclcpp_lifecycle
  std_msgs
  geometry_msgs
  sensor_msgs
  canopen
)

ament_package()
