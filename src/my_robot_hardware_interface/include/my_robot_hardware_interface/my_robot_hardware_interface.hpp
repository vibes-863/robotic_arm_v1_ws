// my_robot_hardware_interface.hpp

#ifndef MY_ROBOT_HARDWARE_INTERFACE_HPP
#define MY_ROBOT_HARDWARE_INTERFACE_HPP

#include <memory>
#include <vector>
#include <string>
#include <hardware_interface/system_interface.hpp>
#include <hardware_interface/types/hardware_interface_type_values.hpp>
#include <rclcpp_lifecycle/state.hpp>
#include <rclcpp/macros.hpp>
#include <hardware_interface/handle.hpp>
#include <hardware_interface/hardware_info.hpp>

namespace my_robot_hardware_interface
{

class MyRobotHardwareInterface : public hardware_interface::SystemInterface
{
public:
  RCLCPP_SHARED_PTR_DEFINITIONS(MyRobotHardwareInterface)

  MyRobotHardwareInterface();

  hardware_interface::CallbackReturn on_init(const hardware_interface::HardwareInfo & info) override;

  std::vector<hardware_interface::StateInterface> export_state_interfaces() override;

  std::vector<hardware_interface::CommandInterface> export_command_interfaces() override;

  hardware_interface::CallbackReturn on_activate(const rclcpp_lifecycle::State & previous_state) override;

  hardware_interface::CallbackReturn on_deactivate(const rclcpp_lifecycle::State & previous_state) override;

  hardware_interface::return_type read(const rclcpp::Time & time, const rclcpp::Duration & period) override;

  hardware_interface::return_type write(const rclcpp::Time & time, const rclcpp::Duration & period) override;

private:
  // Variables to store the joint states and commands
  std::vector<double> hw_positions_;
  std::vector<double> hw_velocities_;
  std::vector<double> hw_efforts_;
  std::vector<double> hw_commands_;

  // Add hardware-specific variables here
};

}  // namespace my_robot_hardware_interface

#endif  // MY_ROBOT_HARDWARE_INTERFACE_HPP
