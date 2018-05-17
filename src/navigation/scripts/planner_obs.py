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
		rospy.Subscriber("destination_gps", Goal, self.goalCallback_gps)
		rospy.Subscriber("destination_ball", Goal, self.goalCallback_ball)


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
		self.state = 0

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
		
		thread.start_new_thread( self.scanning,())

	def scanning (self):
        rate = rospy.Rate(3)
        while not rospy.is_shutdown():
            for i in range(315,405):
                if self.array[i%360] < 5.0:
                    self.t = self.t+1
                    if self.t>2:
                        pass
                        # rospy.loginfo("stop")
                else:
                    self.t = 0
                    rospy.loginfo("go")
                rate.sleep()

	def spin(self):

		r = rospy.Rate(10)
		while not rospy.is_shutdown():
			self.planner()
			r.sleep()
			# print "a"

	def planner(self):
		vel = WheelVelocity()
		planner_status = Planner_msg()
		# bearing_diff = 0
		if self.state ==1:
			if self.dist>self.dist_tolerance:
				planner_status.status = 0
				if self.dist_prev != self.dist:
					print 'turning'
					vel.mode = 2
					if (abs(self.bearing-self.imu_yaw)>self.bearing_tolerance):
						if self.bearing>0 and self.imu_yaw<0 :
							if abs(self.bearing-(self.imu_yaw))>abs(self.bearing-(self.imu_yaw+360)):
								bearing_diff = self.bearing-(self.imu_yaw+360)
							else:
								bearing_diff = self.bearing-(self.imu_yaw)
							# print self.imu_yaw
						elif self.bearing<0 and self.imu_yaw>0 :
							if abs(self.bearing-(self.imu_yaw))>abs((self.bearing+360)-(self.imu_yaw)):
								bearing_diff = (self.bearing+360)-(self.imu_yaw)
							else:
								bearing_diff = self.bearing-(self.imu_yaw)
						else:
							bearing_diff = self.bearing-(self.imu_yaw)

						print bearing_diff
						velocity = self.turn_min + abs(self.bearing-self.imu_yaw)*self.kp_turn
						velocity = velocity if velocity<self.turn_max else self.turn_max
						if (bearing_diff<0):
							vel.left_front_vel  = velocity                 # make the direction correct
							vel.right_front_vel =  -velocity
							vel.left_back_vel   = velocity
							vel.right_back_vel  = - velocity
							print "yaw_diff"
							# print bearing_diff
							print "--------------------"
							if self.stop ==0:
								self.pub_motor.publish(vel)

						elif (bearing_diff>0):
							print "yaw_diff"
							# print bearing_diff
							print "--------------------"
							vel.left_front_vel  =  -velocity                 # make the direction correct
							vel.right_front_vel =  velocity
							vel.left_back_vel   =  -velocity
							vel.right_back_vel  =  velocity
							if self.stop ==0:
								self.pub_motor.publish(vel)
					else :
						vel.mode = 1
						self.dist_prev = self.dist
						vel.left_front_vel  = 0
						vel.right_front_vel = 0
						vel.left_back_vel   = 0
						vel.right_back_vel  = 0
						vel.right_steer     = self.right_steer_zero
						vel.left_steer      = self.left_steer_zero
						if self.stop ==0:
							self.pub_motor.publish(vel)
						r = rospy.Rate(0.5)
						r.sleep()

				elif  self.dist_prev == self.dist:
					vel.mode = 1
					if self.bearing>0 and self.imu_yaw<0 :
						if abs(self.bearing-(self.imu_yaw))>abs(self.bearing-(self.imu_yaw+360)):
							bearing_diff = self.bearing-(self.imu_yaw+360)
						else:
							bearing_diff = self.bearing-(self.imu_yaw)
						# print self.imu_yaw
					elif self.bearing<0 and self.imu_yaw>0 :
						if abs(self.bearing-(self.imu_yaw))>abs((self.bearing+360)-(self.imu_yaw)):
							bearing_diff = (self.bearing+360)-(self.imu_yaw)
						else:
							bearing_diff = self.bearing-(self.imu_yaw)
					else:
						bearing_diff = self.bearing-(self.imu_yaw)

					print bearing_diff
					# print self.bearing-self.imu_yaw
					self.theta_r,self.theta_l,self.vtheta_r,self.vtheta_l = self.steer((self.bearing-self.imu_yaw)/30)
					vel.left_front_vel  = self.forward
					vel.right_front_vel = self.forward
					vel.left_back_vel   = self.forward
					vel.right_back_vel  = self.forward
					vel.right_steer     = self.right_steer_zero #+(math.degrees(self.theta_r)*self.kp_forward)
					vel.left_steer      = self.left_steer_zero #+(math.degrees(self.theta_l)*self.kp_forward)
					if self.stop ==0:
						self.pub_motor.publish(vel)
			else:
				self.state =0

		elif self.state ==2:
			vel.mode = 1
			planner_status.status = 0
			self.pub_planner.publish(planner_status)
			vel.left_front_vel  = 40
			vel.right_front_vel = 40
			vel.left_back_vel   = 40
			vel.right_back_vel  = 40
			vel.right_steer     = self.right_steer_zero #+(math.degrees(self.theta_r)*self.kp_forward)
			vel.left_steer      = self.left_steer_zero #+(math.degrees(self.theta_l)*self.kp_forward)
			if self.stop ==0:
				self.pub_motor.publish(vel)
			r = rospy.Rate(0.2)
			r.sleep()
			self.state =0

		elif self.state==0:
			print "goal reached"
			planner_status.status = 1
			vel.left_front_vel  = 0
			vel.right_front_vel = 0
			vel.left_back_vel   = 0
			vel.right_back_vel  = 0
			vel.right_steer     = self.right_steer_zero
			vel.left_steer      = self.left_steer_zero
			if self.stop ==0:
				self.pub_motor.publish(vel)
			self.pub_planner.publish(planner_status)

		if self.stop ==0:
			self.pub_motor.publish(vel)
		self.pub_planner.publish(planner_status)


	def steer(self,value):
		if(value<0):
			theta_r =  math.radians(min(abs(12.6*value),12.6))
			theta_l = -math.radians(min(abs(20*value),20))
			try:
				radius_of_curve = (self.chassis_length/(2*math.tan(max(abs(self.theta_r),abs(self.theta_l))))) - (self.chassis_width/2)
				vtheta_r = self.chassis_length/(2*math.sin(self.theta_r)*self.radius_of_curve)
				vtheta_l = self.chassis_length/(2*math.sin(self.theta_l)*self.radius_of_curve)
			except:
				pass
		elif(value>0):
			theta_r = -math.radians(min(abs(20*value),20))
			theta_l = math.radians(min(abs(12.6*value),12.6))
			try:
				radius_of_curve = (self.chassis_length/(2*math.tan(max(abs(self.theta_r),abs(self.theta_l))))) - (self.chassis_width/2)
				vtheta_r = self.chassis_length/(2*math.sin(self.theta_r)*self.radius_of_curve)
				vtheta_l = self.chassis_length/(2*math.sin(self.theta_l)*self.radius_of_curve)
			except:
				pass
		else:
			theta_r =0.0
			theta_l =0.0
			vtheta_r=1.0
			vtheta_l=1.0
		return theta_r,theta_l,vtheta_r,vtheta_l

	def imuCallback(self,msg):

		self.imu_yaw=msg.yaw
		self.imu_yaw =-self.imu_yaw


	def goalCallback_gps(self,msg):
		self.bearing = msg.bearing
		self.dist = msg.distance
		self.state = msg.state
		self.stop = msg.stop

	def goalCallback_ball(self,msg):
		# self.bearing = msg.bearing
		self.ball_dist = msg.distance
		self.state = msg.state
		self.stop = msg.stop

if __name__ == '__main__':

	dest = Planner()
	dest.spin()
