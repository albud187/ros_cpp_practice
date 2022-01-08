//libraries
#include <ros/ros.h>
#include <iostream>
#include <algorithm>
#include <math.h>

//msgs
#include <std_msgs/String.h>
#include <std_msgs/Int64.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Pose2D.h>
#include <nav_msgs/Odometry.h>

using namespace std;

//global variables
geometry_msgs::Pose2D current_pose;
geometry_msgs::Pose2D destination;
geometry_msgs::Pose2D delta_pose;
std_msgs::Int64 counter;

//publishers
ros::Publisher deltaPosePub;

//subscribers
ros::Subscriber destinationSub;
ros::Subscriber counterSub;
ros::Subscriber poseSub;

//callback functions
void updateDestination(const geometry_msgs::Pose2D::ConstPtr& msg);
void updateCounter(const std_msgs::Int64::ConstPtr& msg);
void updatePose(const nav_msgs::Odometry::ConstPtr& msg);

//other function prototypes
void display_info();
void get_delta_pose();

//main loop
int main (int argc, char **argv)
{

  ros::init(argc, argv, "navigator");
  ros::NodeHandle nh;
  //subsribers
  poseSub = nh.subscribe("/odom", 1000, updatePose);
  destinationSub =  nh.subscribe("/destination", 1000, updateDestination);
  counterSub = nh.subscribe("/counter", 1000, updateCounter);

  //publishers
  deltaPosePub = nh.advertise<geometry_msgs::Pose2D>("/delta_pose", 1000);

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

void updateDestination(const geometry_msgs::Pose2D::ConstPtr& msg){
  destination.x = msg->x;
  destination.y = msg->y;
}

void updateCounter(const std_msgs::Int64::ConstPtr& msg){
  counter.data = msg->data;
}
struct Quaternion {
    float w, x, y, z;
};

struct EulerAngles {
    float roll, pitch, yaw;
};

EulerAngles ToEulerAngles(Quaternion q) {
    EulerAngles angles;

    // roll (x-axis rotation)
    float sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
    float cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
    angles.roll = std::atan2(sinr_cosp, cosr_cosp);

    // pitch (y-axis rotation)
    float sinp = 2 * (q.w * q.y - q.z * q.x);
    if (std::abs(sinp) >= 1)
        angles.pitch = std::copysign(M_PI / 2, sinp); // use 90 degrees if out of range
    else
        angles.pitch = std::asin(sinp);

    // yaw (z-axis rotation)
    float siny_cosp = 2 * (q.w * q.z + q.x * q.y);
    float cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
    angles.yaw = std::atan2(siny_cosp, cosy_cosp);

    return angles;
}

void updatePose(const nav_msgs::Odometry::ConstPtr& msg){
    current_pose.x = msg->pose.pose.position.x;
    current_pose.y = msg->pose.pose.position.y;

    Quaternion q;

    q.x = msg->pose.pose.orientation.x;
    q.y = msg->pose.pose.orientation.y;
    q.z = msg->pose.pose.orientation.z;
    q.w = msg->pose.pose.orientation.w;

    EulerAngles robot_angles = ToEulerAngles(q);

    current_pose.theta = robot_angles.yaw;
    // current_pose.theta = yaw;

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
