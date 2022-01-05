//libraries
#include <ros/ros.h>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <stdlib.h>
//msgs
#include <std_msgs/Int64.h>

#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Pose2D.h>
#include <sensor_msgs/LaserScan.h>

using namespace std;

//global variables
sensor_msgs::LaserScan scanner_data;
geometry_msgs::Pose2D sensed_object;

//subscribers
ros::Subscriber sensorSub;

//publishers
ros::Publisher sensedObjectPub;

//callback functions
void detect_object(const sensor_msgs::LaserScan msg);
void display_ranges();
//working functions

int main (int argc, char **argv)
{
  ros::init(argc, argv, "sensed_object");
  ros::NodeHandle nh;

  //subscribers
  sensorSub = nh.subscribe("/scan", 100, detect_object);

  //publishers
  sensedObjectPub = nh.advertise<geometry_msgs::Pose2D>("/sensed_object", 100);

  ros::Rate rate(10);

  while(ros::ok()){
    ros::spinOnce();
    display_ranges();
    rate.sleep();
  }
}

void detect_object(const sensor_msgs::LaserScan msg){
  scanner_data = msg;
}


void display_ranges(){
  if (!scanner_data.ranges.empty()){
    for(int j = 340; j<361; j++){
      cout<<"ranges["<<j<<"] = " <<scanner_data.ranges[j]<<endl;
    }
    for(int i = 0; i<21; i++){
      cout<<"ranges["<<i<<"] = " <<scanner_data.ranges[i]<<endl;
    }

    cout<<" "<<endl;


  }


}
