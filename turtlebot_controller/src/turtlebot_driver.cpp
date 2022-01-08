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
geometry_msgs::Pose2D delta_pose;
std_msgs::Int64 counter;
geometry_msgs::Twist Rvel;
geometry_msgs::Twist Tvel;
geometry_msgs::Twist Vel;

float DELTA_THETA_THRESHOLD = 0.15;
float DELTA_P_THRESHOLD = 0.1;

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
void move_to_destination();
int main (int argc, char **argv)
{
  ros::init(argc, argv, "turtlebot_driver");
  ros::NodeHandle nh;

  //subscribers
  deltaPoseSub = nh.subscribe("/delta_pose", 1000, updatePoseDiff);
  counterSub = nh.subscribe("/counter", 1000, updateCounter);

  //publishers
  velPub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

  ros::Rate rate(60);

  while(ros::ok()){
    ros::spinOnce();

    if (counter.data > 0){
      // move_to_destination();
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
      cout<<"stationary"<<endl;
    }
    cout<<position_close_enough()<<endl;
    cout<<delta_pose<<endl;
    rate.sleep();

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

void move_to_destination(){
  float THETA_TH = 0.1;
  float DISTANCE_TH = 0.15;
  float N = 15;
  float target_distance = sqrt(pow(delta_pose.x,2) + pow(delta_pose.y,2));

  if (target_distance > DISTANCE_TH){
    if ( abs(delta_pose.theta) > THETA_TH && delta_pose.theta > 0){
        Vel.linear.x = target_distance/2;
        Vel.angular.z = -abs(delta_pose.theta)/2;
    }
    if (abs(delta_pose.theta) > THETA_TH && delta_pose.theta < 0 ){
        Vel.linear.x = target_distance/2;
        Vel.angular.z = abs(delta_pose.theta)/2;
    }
    velPub.publish(Vel);
  }

  else if( target_distance > DISTANCE_TH && target_distance < DISTANCE_TH*1.5){
    if ( abs(delta_pose.theta) > THETA_TH && delta_pose.theta > 0){
        Vel.linear.x = DISTANCE_TH/N;
        Vel.angular.z = -abs(delta_pose.theta)/2;
    }
    if (abs(delta_pose.theta) > THETA_TH && delta_pose.theta < 0 ){
        Vel.linear.x = DISTANCE_TH/N;
        Vel.angular.z = abs(delta_pose.theta)/2;
    }
    velPub.publish(Vel);
  }

}
