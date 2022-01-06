//libraries
#include <ros/ros.h>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <stdlib.h>
//msgs
#include <std_msgs/String.h>
#include <std_msgs/Int64.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Pose2D.h>
#include <geometry_msgs/Twist.h>

using namespace std;

//global variables
geometry_msgs::Pose2D current_pose;
geometry_msgs::Pose2D delta_pose;
std_msgs::Int64 counter;
geometry_msgs::Twist Rvel;
geometry_msgs::Twist Tvel;

float DELTA_THETA_THRESHOLD = 0.01;
float DELTA_P_THRESHOLD = 0.07;

//publishers
ros::Publisher velPub;

//subscribers
ros::Subscriber deltaPoseSub;
ros::Subscriber currentPoseSub;
ros::Subscriber counterSub;

//callback functions
void updatePoseDiff(const geometry_msgs::Pose2D::ConstPtr& msg);
void updateCounter(const std_msgs::Int64::ConstPtr& msg);

//working functions
bool theta_close_enough();
bool position_close_enough();
void rotation_control();
void translation_control();

void updatePose(const geometry_msgs::Pose2::constPtr &msg) {
  current_pose.x = msg->x;
  current_pose.y = msg->y;
  current_pose.theta = msg->theta;
}


int main (int argc, char **argv)
{
  ros::init(argc, argv, "robot_driver");
  ros::NodeHandle nh;

  //subscribers
  deltaPoseSub = nh.subscribe("turtle1/delta_pose", 1000, updatePoseDiff);
  currentPoseSub = nh.subscribe("turtle1/pose", 1000, updatePose);
  counterSub = nh.subscribe("/turtle1/counter", 1000, updateCounter);

  //publishers
  velPub = nh.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 10);

  ros::Rate rate(20);

  while(ros::ok()){
    ros::spinOnce();

    if (counter.data > 0){
      if (position_close_enough()==true){
        // cout<<"stationary"<<endl;
      }

      if (theta_close_enough() == false && position_close_enough() == false){
        // cout<<"rotating"<<endl;
        rotation_control();
        velPub.publish(Rvel);
      }

      if (position_close_enough()==false && theta_close_enough()==true){
        // cout<<"moving"<<endl;
        translation_control();
        velPub.publish(Tvel);
      }

    }else{
      // cout<<"stationary"<<endl;
    }
    cout<<position_close_enough()<<endl;
    cout<<delta_pose<<endl;
    rate.sleep();

    // vel.linear.x = 1;
    // vel.angular.z = 0.1;

    // cout <<counter<< endl;
    // cout <<" "<< endl;
    // cout<<delta_pose<< endl;
    // velPub.publish(vel);

  }

  return 0;
}

void updatePoseDiff(const geometry_msgs::Pose2D::ConstPtr& msg){
  delta_pose.x = msg->x;
  delta_pose.y = msg->y;
  delta_pose.theta = msg->theta;
}

void updateCounter(const std_msgs::Int64::ConstPtr& msg){
  counter.data = msg->data;
}


bool theta_close_enough(){
  bool output;
  if (abs(delta_pose.theta)< DELTA_THETA_THRESHOLD){
    output = true;
  }else{
    output = false;
  }
  return output;
}

bool position_close_enough(){
  bool output;
  if ( abs(sqrt(pow(delta_pose.x,2)+pow(delta_pose.y,2))) > DELTA_P_THRESHOLD ){
    output = false;
  }else{
    output = true;
  }
  return output;
}

void rotation_control(){
  if(abs(delta_pose.theta)>3){
    Rvel.angular.z = 3;
  }else{
    Rvel.angular.z = delta_pose.theta;
  }
}

void translation_control(){
  if(abs(sqrt(pow(delta_pose.x,2)+pow(delta_pose.y,2)))>=3){
    Tvel.linear.x = 3;
  }else{
    Tvel.linear.x = abs(sqrt(pow(delta_pose.x,2)+pow(delta_pose.y,2)));
  }
}
