# Pre-requisites
## 1. ROS2 Humble 

If you dont have it, you can install it using the following
``` bash
sudo apt update && sudo apt upgrade
sudo apt install ros-humble-desktop
```
After installation, you need to source the ROS 2 environment every time you open a terminal.

To do it for the current session:
```bash
source /opt/ros/humble/setup.bash
```
To make it permanent (automatically sourced in every new terminal), add this line to your ~/.bashrc file:
```bash
echo "source /opt/ros/humble/setup.bash" >> ~/.bashrc
source ~/.bashrc
```

## 2. rosdep
    
Run:
```bash
sudo apt install python3-rosdep 
```
Then initialize `rosdep`:
```bash
sudo rosdep init
rosdep update
```

### What is `rosdep`?

- It is a dependency management tool for ROS. It helps you install required system dependencies for ROS packages automatically.
- When you build a ROS 2 workspace, some packages depend on external system libraries. Instead of manually installing them, rosdep automates this process.

### How to use it?

After installation, initialize rosdep with:
```bash
sudo rosdep init
rosdep update
```

Then, in a workspace, you can run:
```bash
rosdep install --from-paths src --ignore-src -r -y
```
This will install missing dependencies for all packages in your workspace.

## 3. python3-colcon-common-extensions
Run:
```bash
sudo apt install python3-colcon-common-extensions
```
This installs Colcon, the official build system for ROS 2.

### Why is it needed?
- ROS 2 does not use catkin_make (from ROS 1). Instead, it uses colcon to build packages.
- This package includes useful extensions that make working with colcon easier.

### How to use it?
After installation, to build a ROS 2 workspace:
```bash
colcon build
```

To clean and rebuild:
```bash
colcon build --symlink-install --event-handlers console_cohesion+
```

To source the workspace after building:
```bash
source install/setup.bash
```

## 4. MoveIt packages
Install the following one by one:
```bash
sudo apt install ros-humble-moveit
sudo apt install ros-humble-ros2-control ros-humble-ros2-controllers
sudo apt install ros-humble-gripper-controllers
sudo apt install ros-humble-moveit-servo
sudo apt install ros-humble-joy
```




# Steps to move the arm using MoveIt

1. Clone the workspace using the following
    ``` bash
    git clone https://github.com/vibes-863/robotic_arm_v1_ws.git
    ``` 

2. Navigate to the workspace and then make sure all dependencies are installed
    ```bash
    cd robotic_arm_v1_ws
    rosdep install --from-paths src --ignore-src -r -y
    ```
    NOTE: you will get an error saying that `ros-humble-warehouse-ros-mongo` failed to install. That is alright, we don't need it for now as of Feb 21, 2025.

3. Build the workspace
    ```bash
    colcon build
    ```
    NOTE: If the Orin NX crashes, colcon build sequentially (ChatGPT how to do this)

    ALSO NOTE: There will be a bunch of stderr outputs, that can be ignored. It is from the ros2_canopen package.

    After building, source the workspace:
    ```bash
    source install/setup.bash
    ```

4. Make sure that the Node IDs of the motors are as follows:
    - Shoulder motor: 1
    - Elbow motor: 2
    - Wrist right motor: 3
    - Writst left motor: 4

    Note that the Master (the Jetson Orin) is set to Node ID 5

5. Make sure the CAN transceiver is connected to the orin and then run the following:

    DO THE FOLLOWING ONLY IF ITS THE FIRST TIME ON THE ORIN
    ```bash
    sudo busybox devmem 0x0c303018 w 0xc458
    sudo busybox devmem 0x0c303010 w 0xc400
    sudo modprobe can
    sudo modprobe can_raw
    sudo modprobe mttcan
    ```

    Do the following EVERY TIME you want to run the arm:
    ```bash
    sudo ip link set can0 up type can bitrate 1000000
    sudo ip link set can0 txqueuelen 1000
    sudo ip link set can0 up
    ```

    Then, check if CAN0 is up and running using the following:
    ```bash
    ip link show can0
    ```
    You can also run the following to see if CAN0 is there:
    ```bash
    ifconfig
    ```

6. Now lets first get the motors initialized. For this, first make sure that all the motors are connected to each other via power and CAN. Then make sure that the CAN is connected to the orin NX. ONLY AFTER connecting the CAN cables, power on the power supply while making sure that the voltage is at 48V and the current limit is high enough (refer to datasheet).

    Then you can run the following code:
    ```bash
    ros2 launch my_robot_canopen_control robot_control.launch.py
    ```

    You should see in the logs `Successfully Initialized` messages for all the motors and all hear a `tick` sound for each motor. If you don't hear this, stop the program and run it again.

    Once you hear all the four ticks, move to the next step.

7. Now you can launch the arm controlling interface:

    WARNING: IF THIS IS YOUR FIRST TIME CONTROLLING THE ARM, MAKE SURE EITHER VAIBHAV, VJ, OR SHYAM IS THERE WITH YOU AS IF NOT YOU COULD DESTROY THE ARM!

    Run the following:
    ```bash
    ros2 launch my_robot_moveit_config demo.launch.py
    ```

Hopefully, that's it!


Things left to do:
- The inverse kinematics is not working correctly due to the four bar linkage. Possible solution is to design own controller. If that issue is fixed, in step 7, you can use the following command instead:
    ```bash
    ros2 launch my_robot_servo my_robot_servo.launch.py
    ```
    This would result in you being able to control the arm using the PS4 controller.

