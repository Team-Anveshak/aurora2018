#!/usr/bin/env python
import rospy
from sensor_msgs.msg import Joy
from rover_msgs.msg import WheelVelocity


gripper_value=WheelVelocity()

pub=rospy.Publisher("gripper",WheelVelocity, queue_size=10)


def callback(msg):
	gripper_value.left=150*msg.axes[1]
	pub.publish(gripper_value)



def main():
	rospy.init_node("Gripper_node")
	rospy.Subscriber('/joy', Joy, callback)
	rate=rospy.Rate(10)
	while not rospy.is_shutdown():
		rate.sleep()
		rospy.spin
	

if __name__ == '__main__':
	 main()