#!/usr/bin/env python

import rospy
import actionlib
from move_base_msgs.msg import MoveBaseAction, MoveBaseGoal
from nav_msgs.msg import Odometry
from sensor_msgs.msg import NavSatFix
from math import *


initial_lat = 9.30513775
initial_lon = 76.53274845


class GPS() :

	def __init__(self):


		rospy.init_node("gps_goal")

		rospy.Subscriber("odom", Odometry, self.odomCallback)
		rospy.Subscriber("fix", NavSatFix, self.gpsCallback)

		self.client = actionlib.SimpleActionClient('move_base', MoveBaseAction)
		#self.client.wait_for_server()

		self.dest_lat= 12.992424
		self.dest_lon= 80.232159
		self.init_lat=12.992424
		self.init_lon=80.232159
		self.current_x=0.0
		self.current_y=0.0
		self.status=0


	def spin(self):

		r = rospy.Rate(0.33333)

		while not rospy.is_shutdown():

			self.goaler()
			r.sleep()


	def goaler(self):

		lon1, lat1, lon2, lat2 = map(radians, [self.init_lon, self.init_lat, self.dest_lon, self.dest_lat])

		dlon = lon2 - lon1
		dlat = lat2 - lat1
		a = sin(dlat/2)**2 + cos(lat1) * cos(lat2) * sin(dlon/2)**2
		c = 2 * atan2(sqrt(a), sqrt(1-a))
		dist = 6371 * c*1000

		bearing = atan2(sin(lon2-lon1)*cos(lat2), (cos(lat1)*sin(lat2))-(sin(lat1)*cos(lat2)*cos(lon2-lon1)))
		bearing = -degrees(bearing)
		if (bearing<0):
			bearing = bearing+180
		bearing = radians(bearing)


		print ""
		print ""
		print "--------------------"
		print "Horizontal Distance:"
		print dist
		print "--------------------"
		print "Bearing:"
		print bearing
		print "--------------------"

		print degrees(bearing)
		print "--------------------"

		goal = MoveBaseGoal()
		goal.target_pose.header.frame_id = 'odom'
		goal.target_pose.pose.position.x = dist*cos(bearing) + self.current_x
		goal.target_pose.pose.position.y = dist*sin(bearing) + self.current_y
		goal.target_pose.pose.position.z = 0.0
		goal.target_pose.pose.orientation.x = 0.0
		goal.target_pose.pose.orientation.y = 0.0
		goal.target_pose.pose.orientation.z = sin(bearing/2)
		goal.target_pose.pose.orientation.w = cos(bearing/2)

		print "--------------------"
		print "x"
		print dist*cos(bearing) + self.current_x
		print "--------------------"
		print "y"
		print dist*sin(bearing) + self.current_y


		self.client.send_goal(goal)








	def odomCallback(self,msg):

		self.current_x=msg.pose.pose.position.x
		self.current_y=msg.pose.pose.position.y

	def gpsCallback(self,msg):

		self.init_lat = msg.latitude
		self.init_lon = msg.longitude
		self.status=msg.status


if __name__ == '__main__':

	gps = GPS()
	gps.spin()
