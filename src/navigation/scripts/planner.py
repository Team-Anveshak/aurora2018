#!/usr/bin/env python

import rospy
from sensor_msgs.msg import NavSatFix
from rover_msgs.msg import Imu
from rover_msgs.msg import WheelVelocity
from rover_msgs.msg import Goal
from math import *
import time





class Planner() :

	def __init__(self):


		rospy.init_node("gps_goal")
		self.pub_motor = rospy.Publisher('loco/wheel_vel', WheelVelocity,queue_size=10)

		rospy.Subscriber("imu", Imu, self.imuCallback)
		rospy.Subscriber("destination", Goal, self.goalCallback)

		#self.client.wait_for_server()

		self.dest_lat= 12.992424
		self.dest_lon= 80.232159
		self.init_lat=12.992424
		self.init_lon=80.232159
		self.imu_yaw=0.0
		self.status=0
		self.then=time.time()
		self.diff=0
		self.bearing=0
		self.dist = 0



	def spin(self):

		r = rospy.Rate(10)
		while not rospy.is_shutdown():
			self.planner()
			r.sleep()

	def planner(self):
		vel = WheelVelocity()
		if ()
		if (abs(self.bearing-self.imu_yaw)>10):
			if (self.bearing-self.imu_yaw<0):
				vel.left_front_vel  = -35
				vel.right_front_vel  = 20
				print "yaw"
				print self.imu_yaw
				print "--------------------"
			elif (self.bearing-self.imu_yaw>0):
				vel.left_front_vel = 20
				vel.right_front_vel =-20
				print "yaw:"
				print  self.imu_yaw
				print "--------------------"
			self.pub_motor.publish(vel)
		else :
			vel.left_front_vel  = 30
			vel.right_front_vel  = 30
			self.pub_motor.publish(vel)



	def imuCallback(self,msg):

		self.imu_yaw=msg.yaw
		self.imu_yaw = -self.imu_yaw
		if (self.imu_yaw<0):
			self.imu_yaw=self.imu_yaw+360

	def goalCallback(self,msg):

		self.init_lat = msg.latitude
		self.init_lon = msg.longitude
		self.status=msg.status


if __name__ == '__main__':

	dest = Planner()
	dest.spin()
