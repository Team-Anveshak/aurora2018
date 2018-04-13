#!/usr/bin/env python
import rospy
from sensor_msgs.msg import NavSatFix
from rover_msgs.msg import Imu
from rover_msgs.msg import WheelVelocity
from rover_msgs.msg import Planner_msg
from rover_msgs.msg import Goal
import math


class Planner() :

	def __init__(self):


		rospy.init_node("planner")

		self.pub_motor = rospy.Publisher("loco/wheel_vel", WheelVelocity,queue_size=10)
		self.pub_planner = rospy.Publisher("planner_status", Planner_msg,queue_size=10)

		rospy.Subscriber("imu", Imu, self.imuCallback)
		rospy.Subscriber("destination", Goal, self.goalCallback)


		self.dist_tolerance    = float(rospy.get_param('~dist_tolerance', 5))
		self.bearing_tolerance = float(rospy.get_param('~bearing_tolerance', 10))
		self.forward           = float(rospy.get_param('~forward', 40))
		self.turn_min          = float(rospy.get_param('~turn_min', 20))
		self.turn_max          = float(rospy.get_param('~turn_max', 40))
		self.kp_turn		   = float(rospy.get_param('~kp_turn',0.5))
		self.kp_forward		   = float(rospy.get_param('~kp_forward',1.0))



		self.imu_yaw=0.0
		self.bearing=0.0
		self.dist = 0.0
		self.dist_prev= 0.0
		self.stop = 0
		self.state = 'turn'

		self.theta_r = 1.0
		self.theta_l = 1.0
		self.radius_of_curve = 1.0
		self.chassis_width=52.0
		self.chassis_length = 60.0
		self.vtheta_r=0.0
		self.vtheta_l=0.0
		self.straight = 0.0
		self.zero_turn = 0.0
		self.steer_straight = 0.0
		self.steer_vel=0.0
		self.left_steer_vel=0.0
		self.right_steer_vel=0.0
		self.mode=1
		self.right_steer_zero = 390.0
		self.left_steer_zero  = 390.0

		self.filepath = "/home/achu/aurora2018/src/man_ctrl/config/drive_config.txt"
		try:
			self.f=open(self.filepath,'r')
			for l in self.f:

				row = l.split()
				self.right_steer_zero = int(row[0])
				self.left_steer_zero  = int(row[1])
				self.f.close()
		except:
			pass

	def spin(self):

		r = rospy.Rate(10)
		while not rospy.is_shutdown():
			self.planner()
			r.sleep()
			# print "a"

	def planner(self):
		vel = WheelVelocity()
		planner_status = Planner_msg()

		if (self.dist>self.dist_tolerance):
			planner_status.status = 0
			if self.dist_prev != self.dist:
				print 'turning'
				vel.mode = 2
				if (abs(self.bearing-self.imu_yaw)>self.bearing_tolerance):
					velocity = self.turn_min + abs(self.bearing-self.imu_yaw)*self.kp_turn
					velocity = velocity if velocity<self.turn_max else self.turn_max

					if (self.bearing-self.imu_yaw<0):
						vel.left_front_vel  = -velocity                 # make the direction correct
						vel.right_front_vel =  velocity
						vel.left_back_vel   = -velocity
						vel.right_back_vel  =  velocity
						print "yaw_diff"
						print self.bearing-self.imu_yaw
						print "--------------------"

					elif (self.bearing-self.imu_yaw>0):
						print "yaw_diff"
						print self.bearing-self.imu_yaw
						print "--------------------"
						vel.left_front_vel  =  velocity                 # make the direction correct
						vel.right_front_vel = -velocity
						vel.left_back_vel   =  velocity
						vel.right_back_vel  = -velocity
				else :
					vel.mode = 1
					self.dist_prev = self.dist
					vel.left_front_vel  = 0
					vel.right_front_vel = 0
					self.pub_motor.publish(vel)
					r = rospy.Rate(1)
					r.sleep()

			elif  self.dist_prev == self.dist:
				vel.mode = 1
				print self.bearing-self.imu_yaw
				self.steer(self.bearing-self.imu_yaw)
				vel.left_front_vel  = self.forward
				vel.right_front_vel = self.forward
				vel.left_back_vel   = self.forward
				vel.right_back_vel  = self.forward
				vel.right_steer     = self.right_steer_zero+(math.degrees(self.theta_r*4)*self.kp_forward)
				vel.left_steer      = self.left_steer_zero +(math.degrees(self.theta_l*4)*self.kp_forward)
				vel.right_steer     = (math.degrees(self.theta_r)*self.kp_forward*4)
				vel.left_steer      = (math.degrees(self.theta_l)*self.kp_forward*4)

				self.pub_motor.publish(vel)

		else :
			print "goal reached"
			planner_status.status = 1
			vel.left_front_vel  = 0
			vel.right_front_vel = 0
		self.pub_motor.publish(vel)
		self.pub_planner.publish(planner_status)


	def steer(self,value):
		if(value<0):
			self.theta_r = -math.radians(min(abs(0.625*value),12.6))
			self.theta_l = -math.radians(min(abs(0.625*value),20))

			self.radius_of_curve = (self.chassis_length/(2*math.tan(max(abs(self.theta_r),abs(self.theta_l))))) - (self.chassis_width/2)
			self.vtheta_r = self.chassis_length/(2*math.sin(self.theta_r)*self.radius_of_curve)
			self.vtheta_l = self.chassis_length/(2*math.sin(self.theta_l)*self.radius_of_curve)
		elif(value>0):
			self.theta_r = math.radians(min(abs(0.625*value)*value,20))
			self.theta_l = math.radians(min(abs(0.625*value),12.6))

			self.radius_of_curve = (self.chassis_length/(2*math.tan(max(abs(self.theta_r),abs(self.theta_l))))) - (self.chassis_width/2)
			self.vtheta_r = self.chassis_length/(2*math.sin(self.theta_r)*self.radius_of_curve)
			self.vtheta_l = self.chassis_length/(2*math.sin(self.theta_l)*self.radius_of_curve)
		else:
			self.theta_r =0.0
			self.theta_l =0.0
			self.vtheta_r=1.0
			self.vtheta_l=1.0

	def imuCallback(self,msg):

		self.imu_yaw=msg.yaw
		self.imu_yaw =-self.imu_yaw


	def goalCallback(self,msg):

		self.bearing = msg.bearing
		self.dist = msg.distance
		self.stop = msg.stop

if __name__ == '__main__':

	dest = Planner()
	dest.spin()
