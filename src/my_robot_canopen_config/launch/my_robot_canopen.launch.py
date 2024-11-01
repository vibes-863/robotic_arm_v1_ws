import os
from ament_index_python import get_package_share_directory
from launch import LaunchDescription
import launch
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource

def generate_launch_description():
    master_bin_path = os.path.join(
        get_package_share_directory("my_robot_canopen_config"),
        "config",
        "my_robot_bus_config",
        "master.bin",
    )
    if not os.path.exists(master_bin_path):
        master_bin_path = ""

    device_container = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            [
                os.path.join(get_package_share_directory("canopen_core"), "launch"),
                "/canopen.launch.py",
            ]
        ),
        launch_arguments={
            "master_config": os.path.join(
                get_package_share_directory("my_robot_canopen_config"),
                "config",
                "my_robot_bus_config",
                "master.dcf",
            ),
            "master_bin": master_bin_path,
            "bus_config": os.path.join(
                get_package_share_directory("my_robot_canopen_config"),
                "config",
                "my_robot_bus_config",
                "bus.yml",
            ),
            "can_interface_name": "vcan0",
        }.items(),
    )

    return LaunchDescription([device_container])