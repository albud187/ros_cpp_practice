#!/usr/bin/python

import rospy
import tf

from tf.transformations import euler_from_quaternion, quaternion_from_euler

def record_odometry(message):
    global odo_reading
    odo_reading = message

    global robot_pose
    robot_pose.x = odo_reading.pose.pose.position.x
    robot_pose.y = odo_reading.pose.pose.position.y
    orientation_q = odo_reading.pose.pose.orientation
    orientation_list = [orientation_q.x, orientation_q.y, orientation_q.z, orientation_q.w]
    (roll, pitch, yaw) = euler_from_quaternion(orientation_list)
    robot_pose.theta = yaw
