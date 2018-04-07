#!/usr/bin/env python
import rospy
from rover_msgs.msg import WheelVelocity
from rover_msgs.msg import Diag_wheel
from sensor_msgs.msg import Joy
import math

class drive():

    def __init__(self):

        rospy.init_node("drive")

        self.pub_motor = rospy.Publisher("loco/wheel_vel",WheelVelocity,queue_size=10)
        rospy.Subscriber("diag/wheel_vel",Diag_wheel,self.Callback)
        rospy.Subscriber("/joy",Joy,self.joyCallback)

        print "Drive node started"

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
        self.right_steer_zero = 300.0
        self.left_steer_zero  = 300.0

    def spin(self):
        rate = rospy.Rate(10)
        while not rospy.is_shutdown():
            # print "hay"
            self.main()
            rate.sleep()

    def main(self):

        vel = WheelVelocity()

        vel.mode = self.mode
        if(self.mode==0):

            vel.left_front_vel  = 0
            vel.right_front_vel = 0
            vel.left_back_vel   = 0
            vel.right_back_vel  = 0

        if(self.mode==1):
            self.steer(self.steer_straight)
            vel.left_front_vel  = self.straight*self.vtheta_l*100
            vel.right_front_vel = self.straight*self.vtheta_r*100
            vel.left_back_vel   = self.straight*self.vtheta_l*100
            vel.right_back_vel  = self.straight*self.vtheta_r*100
            vel.right_steer     = self.right_steer_zero+(math.degrees(self.theta_r*10)) # In the joyCallback convert the values adding the the constants also there.
            vel.left_steer      = self.left_steer_zero+(math.degrees(self.theta_l*10))

        if(self.mode ==2):
            vel.left_front_vel  = -self.zero_turn*100                 # make the direction correct
            vel.right_front_vel =  self.zero_turn*100
            vel.left_back_vel   = -self.zero_turn*100
            vel.right_back_vel  =  self.zero_turn*100
        if(self.mode ==3):
            vel.right_steer_vel = self.right_steer_vel
            vel.left_steer_vel  = self.left_steer_vel

        self.pub_motor.publish(vel)


    def steer(self,value):
        if(value<0):
            self.theta_r = math.radians(12.5*value)
            self.theta_l = math.radians(20*value)

            self.radius_of_curve = (self.chassis_length/(2*math.tan(max(abs(self.theta_r),abs(self.theta_l))))) - (self.chassis_width/2)
            self.vtheta_r = self.chassis_length/(2*math.sin(self.theta_r)*self.radius_of_curve)
            self.vtheta_l = self.chassis_length/(2*math.sin(self.theta_l)*self.radius_of_curve)
        elif(value>0):
            self.theta_r = math.radians(20*value)
            self.theta_l = math.radians(12.5*value)

            self.radius_of_curve = (self.chassis_length/(2*math.tan(max(abs(self.theta_r),abs(self.theta_l))))) - (self.chassis_width/2)
            self.vtheta_r = self.chassis_length/(2*math.sin(self.theta_r)*self.radius_of_curve)
            self.vtheta_l = self.chassis_length/(2*math.sin(self.theta_l)*self.radius_of_curve)
        else:
            self.theta_r =0.0
            self.theta_l =0.0
            self.vtheta_r=1.0
            self.vtheta_l=1.0







    def joyCallback(self,msg):

        self.straight  = msg.axes[1]
        self.zero_turn = msg.axes[3]
        self.steer_straight = msg.axes[2]

        # print "joy"
        if(msg.buttons[5]==1):
            self.right_steer_vel = 1
        elif(msg.buttons[7]==1):
            self.right_steer_vel = -1
        else:
            self.right_steer_vel = 0


        if(msg.buttons[4]==1):
            self.left_steer_vel = 1
        elif(msg.buttons[6]==1):
            self.left_steer_vel = -1
        else:
            self.left_steer_vel = 0


        if(msg.buttons[0]==1):
            self.mode = 1       #Normal mode
        elif(msg.buttons[3]==1):
            self.mode = 2       #Zero turn mode
        elif(msg.buttons[2]==1):
            self.mode = 3       #Calibrate mode
        elif(msg.buttons[1]==1):
            self.mode = 0       #Emergency stop mode

    def Callback(self,msg):

        self.right_steer_zero = msg.right_pot_zero
        self.left_steer_zero  = msg.left_pot_zero



if __name__ == '__main__':
    run = drive()
    run.spin()
