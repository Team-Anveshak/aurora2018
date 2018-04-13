#!/usr/bin/env python
import string
import rospy
import serial
from rover_msgs.msg import enc
from geometry_msgs.msg import Twist

pwm=0
port='/dev/ttyACM0'
try:
	ser = serial.Serial(port=port, baudrate=57600, timeout=1)
except serial.serialutil.SerialException:
	pass


class Serial(object):
	def __init__(self,ser):
		
		self._ser=ser

	def callback(self,msg):
		pwm=msg.linear.x
		self._ser.write('pwm')
		rospy.loginfo(pwm)
		

def callback(msg):
	rospy.loginfo("ok")
	pwm = msg.linear.x
	#ser.write('pwm')
	




def main():
	rospy.init_node("Serial_node")
	#try:
		#ser = serial.Serial(port=port, baudrate=57600, timeout=1)
	#except serial.serialutil.SerialException:
		#pass
	rospy.sleep(3)
	pub=rospy.Publisher("Encoder",enc, queue_size=10)
	#classcall=Serial(ser)
	rospy.Subscriber('/cmd_vel_mux/input/teleop', Twist, callback)

	
	rate = rospy.Rate(10) # 10hz
	while not rospy.is_shutdown():
		encvalue=enc()
		#encvalue.left=2
		#encvalue.right=3
		line = ser.readline()
		lineparts = string.split(line,',')
		#linesparts = lineparts[0].replace(",","")
		if(lineparts[0]=='e'):
			#encvalue.left=float(lineparts[1])
			#encvalue.right=float(lineparts[2])
			try:
				encvalue.left=float(lineparts[1])
				encvalue.right=float(lineparts[2])
				rospy.loginfo(float(lineparts[1]))
			except (ValueError,IndexError):
				pass
			rospy.loginfo('running')
		pub.publish(encvalue)
		rate.sleep()
		rospy.spin
		ser.write('pwm')
	#rospy.spin


	#classcall=Serial(pub,ser)
	#rospy.Subscriber('/cmd_vel', Twist, classcall.callback)
	#rospy.spin




if __name__ == '__main__':
	try:
		main()
	except rospy.ROSInterruptException:
		pass
