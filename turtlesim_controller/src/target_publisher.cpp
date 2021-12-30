#include <ros/ros.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/Pose.h>

int main (int argc, char **argv)
{
  ros::init(argc, argv, "target_publisher");
  ros::NodeHandle nh;
  ros::Publisher pub = nh.advertise<std_msgs::String>("/turtle1/destination", 10);

  ros::Rate rate(10);

  while(ros::ok()){
    std_msgs::String msg;

    std::cout << "enter a destination (x,y): ";
    std::cin >> msg.data;
    pub.publish(msg);

    rate.sleep();
  }


}
