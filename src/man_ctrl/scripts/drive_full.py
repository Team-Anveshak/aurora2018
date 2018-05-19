#!/usr/bin/env python
import rospy
from rover_msgs.msg import WheelVelocity
from rover_msgs.msg import Diag_wheel
from sensor_msgs.msg import Joy
import numpy
import math

class drive():

    def __init__(self):

        rospy.init_node("drive")

        self.pub_motor = rospy.Publisher("loco/wheel_vel",WheelVelocity,queue_size=10)

        rospy.Subscriber("diag/wheel_vel",Diag_wheel,self.Callback)
        rospy.Subscriber("/joy",Joy,self.joyCallback)

        self.n = 360
        self.array  = numpy.empty(self.n, dtype = float)

        rospy.loginfo("Drive node started")

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
        self.mode=0
        self.right_steer_zero = 390.0
        self.left_steer_zero  = 390.0
        self.imu_yaw = 0
        self.t=0
        self.d=1
        self.cur_bearing = 0
        self.kp_forward = float(rospy.get_param('~kp_forward',2.0))
        self.pan_joy = 0
        self.tilt_joy = 0

        self.filepath = "/home/anveshak/aurora2018/src/man_ctrl/config/drive_config.txt"
        try:
            self.f=open(self.filepath,'r')
            for l in self.f:
                row = l.split()
                self.right_steer_zero = int(row[0])
                self.left_steer_zero  = int(row[1])
            self.f.close()
        except Exception:
            print "Error opening file initially"

    def spin(self):
        rate = rospy.Rate(10)
        while not rospy.is_shutdown():
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
            vel.right_steer     = self.right_steer_zero # In the joyCallback convert the values adding the the constants also there.
            vel.left_steer      = self.left_steer_zero

        if(self.mode==1):
            self.theta_r,self.theta_l = self.steer(self.steer_straight)
            vel.left_front_vel  = -self.straight*(50+self.d*50)
            vel.right_front_vel = self.straight*(50+self.d*50)
            vel.left_back_vel   = -self.straight*(50+self.d*50)
            vel.right_back_vel  = self.straight*(50+self.d*50)
            vel.right_steer     = self.right_steer_zero+(math.degrees(self.theta_r))*4 # In the joyCallback convert the values adding the the constants also there.
            vel.left_steer      = self.left_steer_zero+(math.degrees(self.theta_l))*4


        if(self.mode ==2):
            vel.left_front_vel  =  self.zero_turn*100                 # make the direction correct
            vel.right_front_vel =  self.zero_turn*100
            vel.left_back_vel   =  self.zero_turn*100
            vel.right_back_vel  =  self.zero_turn*100
            vel.right_steer     =  self.right_steer_zero # In the joyCallback convert the values adding the the constants also there.
            vel.left_steer      =  self.left_steer_zero
        if(self.mode ==4):
            vel.left_front_vel  =  -self.zero_turn*100                 # make the direction correct
            vel.right_front_vel =  -self.zero_turn*100
            vel.left_back_vel   =  self.zero_turn*100
            vel.right_back_vel  =  self.zero_turn*100
            vel.right_steer     =  self.right_steer_zero # In the joyCallback convert the values adding the the constants also there.
            vel.left_steer      =  self.left_steer_zero
        if(self.mode ==3):
            try:
                self.f=open(self.filepath,'w')
                self.f.write(str(self.right_steer_zero)+" "+str(self.left_steer_zero))
                vel.right_steer_vel = self.right_steer_vel
                vel.left_steer_vel  = self.left_steer_vel
                self.f.close()
            except Exception:
                print "Error in handling file while calibrating"

        vel.pan_joy = self.pan_joy
        vel.tilt_joy = self.tilt_joy
        self.pub_motor.publish(vel)

    def steer(self,value):
        if(value<0):
            theta_r = -math.radians(min(abs(12.6*value),20))
            theta_l = math.radians(min(abs(20*value),12.6))
            # try:
            #     radius_of_curve = (self.chassis_length/(2*math.tan(max(abs(self.theta_r),abs(self.theta_l))))) - (self.chassis_width/2)
            #     vtheta_r = self.chassis_length/(2*math.sin(self.theta_r)*self.radius_of_curve)
            #     vtheta_l = self.chassis_length/(2*math.sin(self.theta_l)*self.radius_of_curve)
            # except Exception:
            #     pass
        elif(value>=0):
            theta_r = math.radians(min(abs(20*value),12.6))
            theta_l = -math.radians(min(abs(12.6*value),20))
            # try:
            #     radius_of_curve = (self.chassis_length/(2*math.tan(max(abs(self.theta_r),abs(self.theta_l))))) - (self.chassis_width/2)
            #     vtheta_r = self.chassis_length/(2*math.sin(self.theta_r)*self.radius_of_curve)
            #     vtheta_l = self.chassis_length/(2*math.sin(self.theta_l)*self.radius_of_curve)
            # except Exception:
            #     pass
        else:
            theta_r =0.0
            theta_l =0.0
            # vtheta_r=1.0
            # vtheta_l=1.0
        return theta_r,theta_l

    def joyCallback(self,msg):
        self.pan_joy = -msg.axes[4]
        self.tilt_joy = msg.axes[5]
        self.straight  = msg.axes[1]
        self.zero_turn = msg.axes[2]
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
            self.mode = 4       #Emergency stop mode
        elif(msg.buttons[9]==1):
            self.mode = 0

        if self.mode == 1:
            if(msg.buttons[7]==1):
                if self.d <4:
                    self.d = self.d + 1
                print("Max speed is {}".format(100+(self.d-1)*50))
            elif(msg.buttons[6]==1):
                if self.d >1:
                    self.d = self.d - 1
                print("Max speed is {}".format(100+(self.d-1)*50))


    def Callback(self,msg):

        self.right_steer_zero = msg.right_pot_zero
        self.left_steer_zero  = msg.left_pot_zero

    def call(self,msg):
        self.array=msg.ranges

    def imuCallback(self,msg):
		self.imu_yaw=msg.yaw
		self.imu_yaw =-self.imu_yaw



if __name__ == '__main__':
    run = drive()
    run.spin()
