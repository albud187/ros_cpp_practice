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

class SubscribeAndPublish
{

private:
  ros::NodeHandle nh;
  ros::Publisher topicPub;
  ros::Subscriber topicSub;
  geometry_msgs::Pose2D destination;

public:

  void callback(const geometry_msgs::Pose2D& input)
  {
    geometry_msgs::Pose2D output = input;
    cout<<"callback"<<endl;
    //.... do something with the input and generate the output...
    topicPub.publish(destination);
    //rate.sleep();
  }

  //constructor
  SubscribeAndPublish()
  {
    //ros::spinOnce();
    //Topic you want to publish
    topicPub = nh.advertise<geometry_msgs::Pose2D>("/destination", 100);

    //Topic you want to subscribe
    while (ros::ok()){
      ros::spinOnce();
      topicSub = nh.subscribe("/destination", 100, &SubscribeAndPublish::callback, this);
      float input_destination[2];
      cout << "enter a destination (x,y): "<<endl;
      for (int x = 0; x <2; x++){
        cin >> input_destination[x];
      }
      destination.x = input_destination[0];
      destination.y = input_destination[1];
      topicPub.publish(destination);
  }
  }

};//End of class SubscribeAndPublish

int main(int argc, char **argv)
{
  //Initiate ROS
  ros::init(argc, argv, "subscribe_and_publish");

  //Create an object of class SubscribeAndPublish that will take care of everything
  SubscribeAndPublish SAPObject;




  return 0;
}
