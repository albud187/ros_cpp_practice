#include <ros/ros.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/Pose.h>

int main (int argc, char **argv)
{
  ros::init(argc, argv, "robot_driver");
  ros::NodeHandle nh;
  ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 10);

  ros::Rate rate(20);

  while(ros::ok()){
    geometry_msgs::Twist vel;

    vel.linear.x = 1;
    vel.angular.z = 0.1;
    pub.publish(vel);

    rate.sleep();
  }


}
