#include <cstdlib> // Use for the absolute value method abs()
#include <iostream> // Enables command line input and output
#include <math.h>
#include "ros/ros.h" // Necessary header files for ROS
#include "geometry_msgs/Twist.h" // Twist messages (linear & angular velocity)
#include "geometry_msgs/Pose2D.h" // x, y position and theta orientation
#include <std_msgs/Int64.h>

#include "turtlesim/Pose.h" // x, y, theta, linear & angular velocity

// Remove the need to use std:: prefix
using namespace std;

geometry_msgs::Pose2D current_pose; // Current x, y, and theta
turtlesim::Pose destination; //destination
turtlesim::Pose pose_diff; // difference pose
std_msgs::Int64 dest_count;


// This callback function updates the current position and
// orientation of the robot.

void updatePose(const turtlesim::PoseConstPtr &msg) {
  current_pose.x = msg->x;
  current_pose.y = msg->y;
  current_pose.theta = msg->theta;
}

void saveDestination(const turtlesim::PoseConstPtr &msg){
  destination.x = msg ->x;
  destination.y = msg ->y;

}

void calculatePoseDiff(){
  pose_diff.x = destination.x - current_pose.x;
  pose_diff.y = destination.y - current_pose.y;
  float target_angle = atan2(pose_diff.y, pose_diff.x);
  pose_diff.theta = target_angle - current_pose.theta;

}

void updateTargetCount(const std_msgs::Int64ConstPtr &msg){
  dest_count.data = msg -> data;
}

void display_pos(){
  cout << "Current x = " << current_pose.x << endl
       << "Current y = " << current_pose.y <<  endl
       << "Current theta = " << current_pose.theta << endl
       << endl;
}


int main(int argc, char **argv) {

  // Initiate ROS
  ros::init(argc, argv, "pose_reporter");

  // Create the main access point to communicate with ROS
  ros::NodeHandle nh;
  ros::Subscriber currentPoseSub = nh.subscribe("turtle1/pose", 10, updatePose);
  ros::Subscriber destinationSub = nh.subscribe("/turtle1/destination", 10, saveDestination);
  ros::Subscriber destCountSub = nh.subscribe("/turtle1/target_count", 10, updateTargetCount);
  ros::Publisher poseDiffPub = nh.advertise<turtlesim::Pose>("turtle1/pose_diff",10);

  ros::Rate loop_rate(10);

  while (ros::ok()) {

    ros::spinOnce();
    calculatePoseDiff();
    poseDiffPub.publish(pose_diff);

    display_pos();

    // if (dest_count > 0){
    //
    // }




    loop_rate.sleep();
  }

  return 0;
}
