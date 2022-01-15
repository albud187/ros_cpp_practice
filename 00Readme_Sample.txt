Build the package:

1 - open a terminal in /home/ros/catkin_ws

2 - in the terminal enter the command:
catkin create_pkg ast4 rospy

3 - open the folder containing the package and move the .launch file there

4 - create a new folder called "scripts" and move all 3 
.py files there

5 - in the terminal, cd into the folder with the command:
cd ast4/scripts

6 - enter the command:
chmod +x data_reporter.py drive_robot.py sensed_object.py

7 - enter the commands:
cd ~/catkin_ws
catkin build ast4

8 - enter the command:
source ~/catkin_ws/devel/setup.bash

9 - the package is built and each node can be run invidiually
using rosrun. Or all nodes, along with rqt and the simulation
on gazebo can be executed with roslaunch

For roslaunch arguments are:
x - starting x coordinate
y - starting y coordinate
world_name - file name of world, default is empty.world

To find list of worlds for world_name argument:
- open a terminal
- enter command: cd /usr/share/gazebo-9/worlds
- enter command: ls

Examples of roslaunch command:

roslaunch ast4 turtlebug.launch x:=1 y:=2 world_name:="willowgarage.world"
^launches the package in willow garage with turtlebot3 starting at position x = 1 and y = 2

roslaunch ast4 turtlebug.launch x:=1 y:=0 
^launches the package in empty world with turtlebot3 starting at position x = 1 and y = 0


roslaunch ast4 turtlebug.launch x:=-1 y:=-0.5 world_name:="wireless_sensors.world"
^launches the package in wireless_sensors world with turtlebot3 starting at position x = -1 and y = -0.5

