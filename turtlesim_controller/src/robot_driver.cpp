#include <ros/ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Int64.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/Pose.h>
using namespace std;

turtlesim::Pose current_pose;
turtlesim::Pose pose_diff;
geometry_msgs::Twist vel;
std_msgs::Int64 dest_count;

float DELTA_THETA_THRESHOLD = 0.01;
float DELTA_P_THRESHOLD = 0.07;

void updatePose(const turtlesim::PoseConstPtr &msg) {
  current_pose.x = msg->x;
  current_pose.y = msg->y;
  current_pose.theta = msg->theta;
}

void updatePoseDiff(const turtlesim::PoseConstPtr &msg){
  pose_diff.x = msg->x;
  pose_diff.y = msg->y;
  pose_diff.theta = msg->theta;
}


void updateTargetCount(const std_msgs::Int64ConstPtr &msg){
  dest_count.data = msg -> data;
}
int main (int argc, char **argv)
{
  ros::init(argc, argv, "robot_driver");
  ros::NodeHandle nh;
  ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 10);
  ros::Subscriber currentPoseSub = nh.subscribe("turtle1/pose", 10, updatePose);
  ros::Subscriber destCountSub = nh.subscribe("/turtle1/target_count", 10, updateTargetCount);
  ros::Subscriber poseDiffSub = nh.subscribe("turtle1/pose_diff", 10, updatePoseDiff);

  ros::Rate rate(20);

  while(ros::ok()){

    vel.linear.x = 1;
    vel.angular.z = 0.1;

    cout <<dest_count<< endl;
    cout <<" "<< endl;
    cout<<pose_diff<< endl;
    pub.publish(vel);

    rate.sleep();
  }

  return 0;
}
