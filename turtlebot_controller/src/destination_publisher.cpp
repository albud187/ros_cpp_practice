//libraries
#include <ros/ros.h>
#include <iostream>
#include <algorithm>

//msgs
#include <std_msgs/String.h>
#include <std_msgs/Int64.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Pose2D.h>
using namespace std;

//global variables
geometry_msgs::Pose2D destination;
std_msgs::Int64 dest_count;

//publishers
ros::Publisher destinationPub;
ros::Publisher counterPub;

//subscribers
ros::Subscriber destinationSubscriber;
ros::Subscriber counterSubscriber;

//callback functions

int main (int argc, char **argv)
{
  ros::init(argc, argv, "destination_publisher");
  ros::NodeHandle nh;
  destinationPub = nh.advertise<geometry_msgs::Pose2D>("/destination", 1000);
  counterPub = nh.advertise<std_msgs::Int64>("/counter", 1000);

  ros::Rate rate(10);

  while(ros::ok()){
    ros::spinOnce();
    float input_destination[2];
    cout << "enter a destination (x,y): "<<endl;
    for (int x = 0; x <2; x++){
      cin >> input_destination[x];
    }
    dest_count.data+=1;

    destination.x = input_destination[0];
    destination.y = input_destination[1];
    destinationPub.publish(destination);
    counterPub.publish(dest_count);
    // destination.x = 4;
    // destination.y = 2;
    destinationPub.publish(destination);


    rate.sleep();
  }

  return 0;
}
