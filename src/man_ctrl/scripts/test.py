#!/usr/bin/env python

import rospy
import subprocess
import signal

child = subprocess.Popen(["rosrun","man_ctrl","drive"])
#child.wait() #You can use this line to block the parent process untill the child process finished.
print("parent process")
print(child.poll())

rospy.loginfo('The PID of child: %d', child.pid)
print ("The PID of child:", child.pid)

rospy.sleep(15)

# child.send_signal(signal.SIGINT) #You may also use .terminate() method
child.terminate()
