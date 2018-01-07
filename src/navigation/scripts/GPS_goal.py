#!/usr/bin/env python

import rospy
import actionlib
from move_base_msgs.msg import MoveBaseAction, MoveBaseGoal
from math import *


initial_lat = 12.99185471 
initial_lon = 80.23084685
dest_lat = 12.99193799 
dest_lon = 80.2315383

lon1, lat1, lon2, lat2 = map(radians, [initial_lon, initial_lat, dest_lon, dest_lat])

dlon = lon2 - lon1
dlat = lat2 - lat1
a = sin(dlat/2)**2 + cos(lat1) * cos(lat2) * sin(dlon/2)**2
c = 2 * atan2(sqrt(a), sqrt(1-a))
dist = 6371 * c*1000

bearing = atan2(sin(lon2-lon1)*cos(lat2), cos(lat1)*sin(lat2)-sin(lat1)*cos(lat2)*cos(lon2-lon1))
bearing = degrees(bearing)
bearing = (bearing + 360) % 360
bearing = - radians(bearing)


#dist = 10



print sin(lon2-lon1)*cos(lat2)
print cos(lat1)*sin(lat2)-sin(lat1)*cos(lat2)*cos(lon2-lon1)
print "--------------------"
print "Horizontal Distance:"
print dist
print "--------------------"
print "Bearing:"
print degrees(bearing)
print "--------------------"
print "x"
print dist*cos(bearing) 
print "--------------------"
print "y"
print dist*sin(bearing) 



def main() :

	client = actionlib.SimpleActionClient('move_base', MoveBaseAction)
	client.wait_for_server()
	goal = MoveBaseGoal()
	goal.target_pose.header.frame_id = 'odom'
	goal.target_pose.pose.position.x = dist*cos(bearing)
	goal.target_pose.pose.position.y = dist*sin(bearing)
	goal.target_pose.pose.position.z = 0.0
	goal.target_pose.pose.orientation.x = 0.0
	goal.target_pose.pose.orientation.y = 0.0
	goal.target_pose.pose.orientation.z = sin(bearing/2)
	goal.target_pose.pose.orientation.w = cos(bearing/2)

	client.send_goal(goal)
	client.wait_for_result()
	return 'success'
	


if __name__ == '__main__':

	rospy.init_node("GPS_goal")
	main()


	
