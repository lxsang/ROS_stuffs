To run the node you need to have the following packages installed

1. Install ubuntu 14.04
2. Install ROS indigo		
3. Install hokuyo\_node for hokuyo URG-04LX-UG01
	* This node provide the topic /scan from the URG laser
	* from binaries: `sudo apt-get install ros-indigo-hokuyo-node`
	* from sources:

	```bash
	# Generic way to install a catkin package from sources
	cd ~/catkin_ws/src
	git clone https://github.com/ros-drivers/hokuyo_node.git
	cd ~/catkin_ws
	catkin_make
	```	
4. Install kobuki package kobuki_node 
	* from binaries as ROS packages : http://wiki.ros.org/kobuki/Tutorials/Installation
	* or from sources https://github.com/yujinrobot/kobuki.git (Same steps above)
5. Ensure turtle_tf package is installed
	* This node provides the transformation from "laser" to "base_link" frame
6. Run `rosrun kobuki_ftdi create_udev_rules` before plugging in the kobuki
7. Install gmapping and move_base

	```bash
		sudo apt-get install ros-indigo-gmapping ros-indigo-move-base
	```
8. Add current user to dialout and restart the machine

	```bash
		sudo usermod -a -G dialout dev 
	```
9. Install the catkin package turtlebot_car
	* https://github.com/CARMinesDouai/turtlebot_car.git (Same steps above)
	* the provided launch file turtlebot_minimal.launch launches the hokuyo_node with specific paramaters that set up larger values for the min and the max angles (more than 180deg)

10. For turtlebot teleoperation using joystick
  * Install turtlebot_teleop in turtlebot package (https://github.com/turtlebot/turtlebot.git)
  * Install joy package (https://github.com/ros-drivers/joystick_drivers.git)
  * Install xbox 360 controller driver ubuntu
    sudo apt-add-repository ppa:rael-gc/ubuntu-xboxdrv
    sudo apt-get update && sudo apt-get install ubuntu-xboxdrv
  * you may need to restart your PC
  * you may need to configure yuor joystick (http://wiki.ros.org/joy/Tutorials/ConfiguringALinuxJoystick#Configuring_the_Joystick)

11. Install PhaROS

	```bash
	curl http://car.mines-douai.fr/scripts/Robulab | bash
	```

