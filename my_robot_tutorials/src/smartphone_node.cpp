#include <ros/ros.h>
#include <std_msgs/String.h>

void callback_recieve_radio_data(const std_msgs::String& msg)
{
  ROS_INFO("Message recieved : %s", msg.data.c_str());
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "smartphone");
  ros::NodeHandle nh;

  ros::Subscriber sub = nh.subscribe("/robot_news_radio", 1000, callback_recieve_radio_data);

  ros::spin();


}
