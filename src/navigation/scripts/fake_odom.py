#!/usr/bin/env python



import rospy
from geometry_msgs.msg import Twist
from nav_msgs.msg import Odometry
from geometry_msgs.msg import Quaternion
from tf.broadcaster import TransformBroadcaster
from math import *


  


dx=0
dw=0



def main():

	odomPub=rospy.Publisher("odomet", Odometry,queue_size=10)
	rospy.Subscriber("cmd_vel",Twist,Odomcallback)

	then = rospy.Time.now()

	d=0.0
	theta=0.0
	x=0
	y=0
	Gx=0
	Gy=0
	global dx
	global dw


	while not rospy.is_shutdown():
		now = rospy.Time.now()
		elapsed = now - then
		then=now
		elapsed = elapsed.to_sec()

		

		d = d + dx*elapsed
		theta = theta+( dw*elapsed)


		x = cos( theta ) * d
		y = -sin( theta ) * d
		Gx = Gx + ( cos( theta ) * x - sin( theta ) * y )
		Gy = Gy + ( sin( theta) * x + cos( theta ) * y )

		quaternion = Quaternion()
		quaternion.x = 0.0
		quaternion.y = 0.0
		quaternion.z = sin( theta / 2 )
		quaternion.w = cos( theta / 2 )

		odom = Odometry()
		odom.header.stamp = now
		odom.header.frame_id = "odom"
		odom.pose.pose.position.x =Gx
		odom.pose.pose.position.y = Gy
		odom.pose.pose.position.z = 0
		odom.pose.pose.orientation = quaternion
		odom.child_frame_id = "chassis"
		odom.twist.twist.linear.x = dx
		odom.twist.twist.linear.y = 0
		odom.twist.twist.angular.z = dw
		odomPub.publish(odom)

		r = rospy.Rate(10)
		
		rospy.loginfo("x= %s " % Gx)
		rospy.loginfo("y= %s " % Gy)
		rospy.loginfo("theta %s " % theta)
		r.sleep()
		


		




def Odomcallback(msg):

	global dx
	global dw

	dx = msg.linear.x
	dw = msg.angular.z




if __name__ == '__main__':
	rospy.init_node("fake_odom")

	
	main()



