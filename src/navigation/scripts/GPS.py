#!/usr/bin/env python

import rospy
from sensor_msgs.msg import NavSatFix
from rover_msgs.msg import Imu
from rover_msgs.msg import WheelVelocity
from math import *
import time


initial_lat = 9.30513775
initial_lon = 76.53274845


class GPS() :

	def __init__(self):


		rospy.init_node("gps_goal")
		self.pub_motor = rospy.Publisher('rover/wheel_vel', WheelVelocity,queue_size=10)

		rospy.Subscriber("imu", Imu, self.imuCallback)
		rospy.Subscriber("fix", NavSatFix, self.gpsCallback)

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


		while not rospy.is_shutdown():
			self.diff=int(time.time()-self.then)
			if(abs(self.diff)>5):
				self.cal()
				self.then=time.time()
			self.goaler()
	def cal(self):

		lon1, lat1, lon2, lat2 = map(radians, [self.init_lon, self.init_lat, self.dest_lon, self.dest_lat])
		dlon = lon2 - lon1
		dlat = lat2 - lat1
		a = sin(dlat/2)**2 + cos(lat1) * cos(lat2) * sin(dlon/2)**2
		c = 2 * atan2(sqrt(a), sqrt(1-a))
		dist = 6371 * c*1000
		self.dist = dist

		bearing = atan2(sin(lon2-lon1)*cos(lat2), (cos(lat1)*sin(lat2))-(sin(lat1)*cos(lat2)*cos(lon2-lon1)))
		bearing = degrees(bearing)
		if (bearing<0):
			bearing = bearing+360
		self.bearing = bearing
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

		print "--------------------"
		print "x"
		print dist*cos(bearing)
		print "--------------------"
		print "y"
		print dist*sin(bearing)

	def goaler(self):
		r = rospy.Rate(10)
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
		r.sleep()



	def imuCallback(self,msg):

		self.imu_yaw=msg.yaw
		self.imu_yaw = -self.imu_yaw
		if (self.imu_yaw<0):
			self.imu_yaw=self.imu_yaw+360

	def gpsCallback(self,msg):

		self.init_lat = msg.latitude
		self.init_lon = msg.longitude
		self.status=msg.status


if __name__ == '__main__':

	gps = GPS()
	gps.spin()
