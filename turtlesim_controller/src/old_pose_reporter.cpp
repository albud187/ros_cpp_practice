#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "turtlesim/Pose.h"
#include <sstream>
#include <iostream> // Enables command line input and output
using namespace std;

ros::Subscriber pose_subscriber;	// to determine the position for turning the robot in an absolute orientation --> in the setDesiredOrientation fn
turtlesim::Pose turtlesim_pose;

const double PI = 3.14159265359;

void poseCallback(const turtlesim::Pose::ConstPtr& pose_message);	//Callback fn everytime the turtle pose msg is published over the /turtle1/pose topic.
void display_pos();


int main(int argc, char **argv)
{
	ros::init(argc, argv, "pose_reporter");
	ros::NodeHandle nh;

	pose_subscriber = nh.subscribe("/turtle1/pose", 10, poseCallback);	//call poseCallback everytime the turtle pose msg is published over the /turtle1/pose topic.
	ros::Rate rate(10);

  while(ros::ok()){
    display_pos();
    rate.sleep();
  }

	ros::spin();

	return 0;
}

void poseCallback(const turtlesim::Pose::ConstPtr& pose_message){
  ROS_INFO("%f", pose_message->x);
	turtlesim_pose.x=pose_message->x;
	turtlesim_pose.y=pose_message->y;
	turtlesim_pose.theta=pose_message->theta;

}

// void poseCallback(const turtlesim::Pose::ConstPtr &pose_msg) {
//     // same as...: turtle_pose.theta = pose_msg->theta; /*and so on for other vars*/
//     turtlesim_pose = *pose_msg;
// }

void display_pos(){
  float xpos = turtlesim_pose.x;
  float ypos = turtlesim_pose.y;
  float theta =  turtlesim_pose.theta;


  ROS_INFO("%f", xpos);
  ROS_INFO("%f", ypos);
  ROS_INFO("%f", theta);
  printf("\n");

}
