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
turtlesim::Pose current_pose;
turtlesim::Pose destination;
turtlesim::Pose delta_pose;
std_msgs::Int64 counter;

//publishers
ros::Publisher deltaPosePub;

//subscribers
ros::Subscriber destinationSub;
ros::Subscriber counterSub;
ros::Subscriber poseSub;

//callback functions
void updateDestination(const turtlesim::Pose::ConstPtr& msg);
void updateCounter(const std_msgs::Int64::ConstPtr& msg);
void updatePose(const turtlesim::Pose::ConstPtr& msg);

//other function prototypes
void display_info();
void get_delta_pose();

//main loop
int main (int argc, char **argv)
{

  ros::init(argc, argv, "target_listener");
  ros::NodeHandle nh;
  //subsribers
  poseSub = nh.subscribe("/turtle1/pose", 1000, updatePose);
  destinationSub =  nh.subscribe("/turtle1/destination", 1000, updateDestination);
  counterSub = nh.subscribe("/turtle1/counter", 1000, updateCounter);

  //publishers
  deltaPosePub = nh.advertise<turtlesim::Pose>("/turtle1/delta_pose", 1000);

  ros::Rate rate(10);

  while(ros::ok()){
    ros::spinOnce();
    get_delta_pose();

    if (counter.data>0){
      deltaPosePub.publish(delta_pose);
    }

    display_info();
    rate.sleep();
  }

  return 0;
}

void updateDestination(const turtlesim::Pose::ConstPtr& msg){
  destination.x = msg->x;
  destination.y = msg->y;
}

void updateCounter(const std_msgs::Int64::ConstPtr& msg){
  counter.data = msg->data;
}

void updatePose(const turtlesim::Pose::ConstPtr& msg){
    current_pose.x = msg->x;
    current_pose.y = msg->y;
    current_pose.theta = msg->theta;

}

void get_delta_pose(){
  delta_pose.x = destination.x - current_pose.x;
  delta_pose.y = destination.y - current_pose.y;
  float target_angle = atan2(delta_pose.y, delta_pose.x);
  delta_pose.theta = target_angle - current_pose.theta;
}

void display_info(){
  cout<<"counter: "<< counter.data<<endl;
  cout<<"current pose"<<endl;
  cout<<current_pose<<endl;
  cout<<""<<endl;

  cout<<"destination"<<endl;
  cout<<destination<<endl;

  cout<<"delta_pose"<<endl;
  cout<<delta_pose<<endl;
}
