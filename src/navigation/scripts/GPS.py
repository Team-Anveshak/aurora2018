#!/usr/bin/env python
12.991221512/80.23294641
12.99186955/80.23278201

import rospy
from sensor_msgs.msg import NavSatFix
from rover_msgs.msg import Imu
from rover_msgs.msg import WheelVelocity
from math import *
import time


# initial_lat = 9.30513775
# initial_lon = 76.53274845


class GPS() :

	def __init__(self):


		file_path = "/home/achu/aurora2018/src/navigation/config/gps_data.txt"
		self.f=open(file_path,'r')
		self.dest_lat_cont,self.dest_lon_cont = [],[]
		for l in self.f:
			row = l.split()
			self.dest_lat_cont.append(row[0])
			self.dest_lon_cont.append(row[1])
		rospy.init_node("gps_goal")
		self.pub_motor = rospy.Publisher('loco/wheel_vel', WheelVelocity,queue_size=10)

		rospy.Subscriber("imu", Imu, self.imuCallback)
		rospy.Subscriber("fix", NavSatFix, self.gpsCallback)

		#self.client.wait_for_server()

		self.dest_lat= float(self.dest_lat_cont[0])
		self.dest_lon= float(self.dest_lon_cont[0])
		self.init_lat=0.0
		self.init_lon=0.0
		self.imu_yaw=0.0
		self.status=0
		self.then=time.time()
		self.diff=0
		self.bearing=0
		self.dist =0.0
		self.n =0



	def spin(self):


		while not rospy.is_shutdown():
			self.diff=int(time.time()-self.then)
			if(abs(self.diff)>1):
				self.dest_lat= float(self.dest_lat_cont[self.n])
				self.dest_lon= float(self.dest_lon_cont[self.n])
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
		if (self.init_lat != 0.0):
			if (self.dist>5):
				if (abs(self.bearing-self.imu_yaw)>10):
					if (self.bearing-self.imu_yaw<0):
						vel.left_front_vel  = 45
						vel.right_front_vel  = -45
						print "yaw"
						print self.imu_yaw
						print "--------------------"
					elif (self.bearing-self.imu_yaw>0):
						vel.left_front_vel = -45
						vel.right_front_vel =45
						print "yaw:"
						print  self.imu_yaw
						print "--------------------"
					self.pub_motor.publish(vel)
				else :
					vel.left_front_vel  = -80
					vel.right_front_vel  = -80
					self.pub_motor.publish(vel)
				r.sleep()
			else :
				print "reached"
				if (self.n<(len(self.dest_lat_cont)-1)):
					self.n = self.n+1
				vel.left_front_vel  = 0
				vel.right_front_vel = 0
				self.pub_motor.publish(vel)




	def imuCallback(self,msg):

		self.imu_yaw=msg.yaw
		self.imu_yaw = -self.imu_yaw


	def gpsCallback(self,msg):

		self.init_lat = msg.latitude
		self.init_lon = msg.longitude
		self.status=msg.status


if __name__ == '__main__':

	gps = GPS()
	gps.spin()
