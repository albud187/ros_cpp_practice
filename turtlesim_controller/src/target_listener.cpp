//libraries
#include <ros/ros.h>
#include <iostream>
#include <algorithm>
#include <math.h>

//msgs
#include <std_msgs/String.h>
#include <std_msgs/Int64.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/Pose.h>

using namespace std;

//global variables
turtlesim::Pose destination;
turtlesim::Pose current_pose;
std_msgs::Int64 dest_count;

//publishers
ros::Publisher destinationPub;
ros::Publisher counterPub;

//subscribers
ros::Subscriber destinationSub;
ros::Subscriber counterSub;

//callback functions
void updateDestination(const turtlesim::Pose::ConstPtr& msg);
void updateCounter(const std_msgs::Int64::ConstPtr& msg);
void updatePose(const turtlesim::Pose::ConstPtr& msg);

//other function prototypes
//void display_infos();
int main (int argc, char **argv)
{
  ros::init(argc, argv, "target_listener");
  ros::NodeHandle nh;
  destinationSub =  nh.subscribe("/turtle1/destination", 1000, updateDestination);
  counterSub = nh.subscribe("/turtle1/counter", 1000, updateCounter);

  ros::Rate rate(10);

  while(ros::ok()){
    cout<<"current pose"<<endl;
    cout<<current_pose<<endl;

    rate.sleep();

  }

  return 0;
}

void updateDestination(const turtlesim::Pose::ConstPtr& msg){
  destination.x = msg->x;
  destination.y = msg->y;
}

void updateCounter(const std_msgs::Int64::ConstPtr& msg){
  dest_count.data = msg->data;
}

void updatePose(const turtlesim::Pose::ConstPtr& msg) {
  current_pose.x = msg->x;
  current_pose.y = msg->y;
  current_pose.theta = msg->theta;
}
