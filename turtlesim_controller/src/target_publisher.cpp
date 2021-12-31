#include <ros/ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Int64.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/Pose.h>
#include <iostream>
#include <algorithm>

using namespace std;

int counter = 0;

turtlesim::Pose destination;
std_msgs::Int64 dest_count;


int main (int argc, char **argv)
{
  ros::init(argc, argv, "target_publisher");
  // ros::NodeHandle nh;

  ros::Publisher pub = nh.advertise<turtlesim::Pose>("/turtle1/destination", 10);
  ros::Publisher counterpub = nh.advertise<std_msgs::Int64>("/turtle1/target_count", 10);
  // ros::Rate rate(10);

  while(ros::ok()){
    ros::Subscriber destinationSub = nh.subscribe("/turtle1/destination", 10, dest_subpub);
    ros::Subscriber destinationCountSubPub = nh.subscribe("/turtle1/target_count", 10, dest_count_subpub);

    float input_destination[2];
    cout << "enter a destination (x,y): "<<endl;
    for (int x = 0; x <2; x++){
      cin >> input_destination[x];
    }
    dest_count.data += 1;

    destination.x = input_destination[0];
    destination.y = input_destination[1];
    pub.publish(destination);
    counter +=1;
    dest_count.data = counter;

    counterpub.publish(dest_count);

    rate.sleep();
  }


}

void dest_subpub(turtlesim::Pose dest_msg){
  ros::Publisher pub = nh.advertise<turtlesim::Pose>("/turtle1/destination", 10);
  pub.publish(destination);
  rate.sleep();
}

void dest_count_subpub(std_msgs::Int64 dest_count_msg){
  ros::Publisher counterpub = nh.advertise<std_msgs::Int64>("/turtle1/target_count", 10);
  counterpub.publish(dest_count);
  rate.sleep();
}
