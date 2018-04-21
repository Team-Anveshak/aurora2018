#!/usr/bin/env python
import rospy
from sensor_msgs.msg import NavSatFix
from rover_msgs.msg import Goal
from rover_msgs.msg import Planner_msg
from math import *
import time
import subprocess


class GPS() :

	def __init__(self):

		rospy.init_node("gps_goal")

		self.pub_goal = rospy.Publisher('destination_gps', Goal,queue_size=10)

		rospy.Subscriber("fix", NavSatFix, self.gpsCallback)
		rospy.Subscriber("planner_status",Planner_msg, self.plannerCallback)
		rospy.Subscriber("destination_ball", Goal, self.goalCallback_ball)

		file_path = "/home/achu/aurora2018/src/navigation/config/gps_data.txt"
		self.f=open(file_path,'r')

		self.dest_lat_cont,self.dest_lon_cont = [],[]
		for l in self.f:
			row = l.split()
			self.dest_lat_cont.append(row[0])
			self.dest_lon_cont.append(row[1])



		self.dest_lat= float(self.dest_lat_cont[0])
		self.dest_lon= float(self.dest_lon_cont[0])
		self.init_lat= 2.324234
		self.init_lon= 0.0
		self.pub= float(rospy.get_param('~publish',10))
		self.then=time.time()
		self.diff=0
		self.bearing=0
		self.planner_status = 0
		self.n=0
		self.mode='gps'
		self.flag=1
		self.dist_tolerance    = float(rospy.get_param('~dist_tolerance', 5))
		self.dist_gps=float(self.dist_tolerance+2.0)
		self.balldistance =1.0
		self.count=0



	def spin(self):
		goal=Goal()
		while not rospy.is_shutdown():
			self.dist_gps,self.bearing=self.cal()
			self.diff=int(time.time()-self.then)
			if self.mode =='gps' and self.flag==1:
				if (self.dist_gps<self.dist_tolerance):
					goal.state = 0
					self.pub_goal.publish(goal)

					self.mode =='ball'
					self.flag = 0
					# self.planner_status = 0
					self.dest_lat= float(self.dest_lat_cont[self.n])
					self.dest_lon= float(self.dest_lon_cont[self.n])

					print '==============changing=============='
					if (self.n<(len(self.dest_lat_cont)-1)):
						self.n = self.n+1
					else :
						print '============stop================'
				if(abs(self.diff)>self.pub):
					print "Horizontal Distance:"
					print self.dist_gps
					print "--------------------"
					print "Bearing:"
					print self.bearing
					print "--------------------"
					print "========lat============"
					print self.dest_lat
					if self.mode=='gps':
						goal.distance=self.dist_gps
						goal.bearing=self.bearing
						goal.state =1
						if (self.init_lat != 0.0):
							self.pub_goal.publish(goal)
					self.then=time.time()

			elif self.mode =='ball' and self.planner_status ==1:
				if self.count>4 or self.balldistance < 0.5:
					self.flag = 1
					self.mode ='gps'
					self.count=0
				else:
					self.count=self.count+1
					goal.state == 0
					self.pub_goal.publish(goal)
					goal.stop = 1
					self.pub_goal.publish(goal)
					try:
						child = subprocess.Popen(["rosrun","navigation","ball_det.py"])
						child.wait()
					except:
						print "cannot open ball_det node"
					self.planner_status = 0


	def cal(self):

		# goal=Goal()
		lon1, lat1, lon2, lat2 = map(radians, [self.init_lon, self.init_lat, self.dest_lon, self.dest_lat])
		dlon = lon2 - lon1
		dlat = lat2 - lat1
		a = sin(dlat/2)**2 + cos(lat1) * cos(lat2) * sin(dlon/2)**2
		c = 2 * atan2(sqrt(a), sqrt(1-a))
		dist_gps = 6371 * c*1000


		bearing = atan2(sin(lon2-lon1)*cos(lat2), (cos(lat1)*sin(lat2))-(sin(lat1)*cos(lat2)*cos(lon2-lon1)))
		bearing = degrees(bearing)
		# goal.bearing = bearing
		# goal.distance = self.dist_gps
		# goal.stop = 0
		# if (self.init_lat != 0.0):
		# 	self.pub_goal.publish(goal)

		# print "--------------------"

		return dist_gps,bearing




	def gpsCallback(self,msg):

		self.init_lat = msg.latitude
		self.init_lon = msg.longitude
		self.status=msg.status

	def plannerCallback(self,msg):
		self.planner_status = msg.status
	def goalCallback_ball(self,msg):
		# self.bearing = msg.bearing
		self.balldistance = msg.distance
		# self.stop = msg.stop



if __name__ == '__main__':

	gps = GPS()
	gps.spin()
