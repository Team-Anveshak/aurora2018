#!/usr/bin/env python
import rospy
from sensor_msgs.msg import NavSatFix
from rover_msgs.msg import Imu
from rover_msgs.msg import WheelVelocity
from rover_msgs.msg import Planner_msg
from rover_msgs.msg import Goal


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
		self.kp				   = float(rospy.get_param('~kp',0.5))


		self.imu_yaw=0.0
		self.bearing=0.0
		self.dist = 0.0
		self.stop = 0
		self.state = 'turn'

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
			if self.state == 'turn':

				if (abs(self.bearing-self.imu_yaw)>self.bearing_tolerance):

					velocity = self.turn_min + abs(self.bearing-self.imu_yaw)*self.kp
					velocity = velocity if velocity<self.turn_max else self.turn_max

					if (self.bearing-self.imu_yaw<0):

						vel.left_front_vel  =  velocity
						vel.right_front_vel = -velocity
						print "yaw"
						print self.bearing-self.imu_yaw
						print "--------------------"
					elif (self.bearing-self.imu_yaw>0):
						print "yaw"
						print self.bearing-self.imu_yaw
						print "--------------------"
						vel.left_front_vel  = -velocity
						vel.right_front_vel =  velocity
				else :
					self.state = 'forward'
					vel.left_front_vel  = 0
					vel.right_front_vel = 0
					self.pub_motor.publish(vel)
					r = rospy.Rate(1)
					r.sleep()

			elif self.state == 'forward':
				print "forward"
				if (abs(self.bearing-self.imu_yaw)<self.bearing_tolerance):
					vel.left_front_vel  = self.forward
					vel.right_front_vel = self.forward
				else :
					self.state ='turn'
					vel.left_front_vel  = 0
					vel.right_front_vel = 0
					self.pub_motor.publish(vel)
					r = rospy.Rate(1)
					r.sleep()

		else :

			planner_status.status = 1
			vel.left_front_vel  = 0
			vel.right_front_vel = 0
		self.pub_motor.publish(vel)
		self.pub_planner.publish(planner_status)




	def imuCallback(self,msg):

		self.imu_yaw=msg.yaw
		self.imu_yaw = -self.imu_yaw


	def goalCallback(self,msg):

		self.bearing = msg.bearing
		self.dist = msg.distance
		self.stop = msg.stop

if __name__ == '__main__':

	dest = Planner()
	dest.spin()
