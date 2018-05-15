from geometry_msgs.msg import Vector3
from sensor_msgs.msg import Joy
import rospy

pub = rospy.Publisher('set', Vector3, queue_size = 100)

def joycallback(joy):
	if joy.buttons[3] is 1:
		set_value = Vector3(-500, -500, 1)
	elif joy.buttons[1] is 1:
		set_value = Vector3(500, 500, 1)
	elif joy.buttons[0] is 1:
		set_value = Vector3(500, -500, 1)
	elif joy.buttons[2] is 1:
		set_value = Vector3(-500, 500, 1)
	elif joy.buttons[6] is 1:
		set_value = Vector3(100, 1, 2)
	elif joy.buttons[7] is 1:
		set_value = Vector3(50, -1, 2)
	elif joy.axes[5] > 0.5:
		set_value = Vector3(90, 90, 3)
	elif joy.axes[5] < -0.5:
		set_value = Vector3(-90, -90, 3)
	elif joy.axes[4] > 0.5:
		set_value = Vector3(90, -90, 3)
	elif joy.axes[4] < -0.5:
		set_value = Vector3(-90, 90, 3)
	elif joy.buttons[4] == 1:
		set_value = Vector3(100, -1, 4)
	elif joy.buttons[5] == 1:
		set_value = Vector3(100, 1, 4)
	else:
		set_value = Vector3(0, 0, 0)
	pub.publish(set_value)

def Start():
	rospy.init_node('Arm_pid_tuner', anonymous = True)
	rospy.Subscriber("joy", Joy, joycallback)
	rospy.spin()

if __name__ == '__main__':
	try:
		Start()
	except  rospy.ROSInterruptException:
		pass
