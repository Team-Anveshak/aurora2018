#!/usr/bin/env python
import rospy
import string
import numpy as np
import cv2
import sys
import math
from sensor_msgs.msg import NavSatFix
from rover_msgs.msg import Imu_yaw

y = 0.0
x = 0.0
ang = 0.0

w = 2749
h = 2749

y1 = 13.007100
y2 = 12.982585
x1 = 80.220812
x2 = 80.244238

imagepath1 = "/home/achu/aurora2018/src/mapping/scripts/IITM.jpeg"
imagepath2 = "/home/achu/aurora2018/src/mapping/scripts/Image.jpg"
filepath   = "/home/achu/aurora2018/src/mapping/scripts/coordinates.txt"

img1 = cv2.imread(imagepath1,1)
l=len(sys.argv)
if(l>=2):
    while (l>0):
        l=l-2
        q = float (sys.argv[1])
        p = float (sys.argv[2])
        x3 = int(w*(p-x1)/(x2-x1))
        y3 = int(h*(q-y1)/(y2-y1))
        cv2.circle(img1,(x3,y3), 1, (255,0,0), -1)
        cv2.circle(img1,(x3,y3), 15, (255,0,0), 10)
try:
    fileobj = open(filepath,"r")
    for i in fileobj.readlines():
        c=i.split(",")
        q = float (c[0])
        p = float (c[1])
        x3 = int(w*(p-x1)/(x2-x1))
        y3 = int(h*(q-y1)/(y2-y1))
        cv2.circle(img1,(x3,y3), 1, (255,0,0), -1)
        cv2.circle(img1,(x3,y3), 15, (255,0,0), 10)
    fileobj.close()
except Exception:
    pass


cv2.imwrite(imagepath2,img1)

class GPSsubscriber():
    def __init__(self):
        rospy.init_node('this_is_subscriber', anonymous=False)
        rospy.Subscriber("fix", NavSatFix,self.callback_coordinate)
        rospy.Subscriber("imu",Imu_yaw,self.callback_angle)
        self.stuff = 0

    def callback_angle(self,values):
        global ang
        ang=values.yaw
        ang = math.pi * ang / 180.0
        #print ang

    def spin(self):
        global x,y,ang, imagepath2
        while not rospy.is_shutdown():
            img = cv2.imread(imagepath2,1)
            print ang
            a = int(w*(x-x1)/(x2-x1))
            b = int(h*(y-y1)/(y2-y1))
            c = int(30.0 * math.sin(ang))
            d = int(30.0 * math.cos(ang))
            cv2.circle(img,(a,b), 1, (0,0,255), -1)
            cv2.circle(img,(a,b), 15, (0,0,255), 3)
            cv2.arrowedLine(img,(a+c,b-d), (a+2*c,b-2*d), (0,0,255), 7, 8, 0, 0.3)

            cv2.namedWindow('image',cv2.WINDOW_NORMAL)
            cv2.resizeWindow('image', 800,800)
            cv2.imshow('image',img)
            cv2.waitKey(5)

    def callback_coordinate(self, stuff):
        global x,y
        y3=str(stuff.latitude)
        x3=str(stuff.longitude)
        if x3 != "nan" and x3!='' and y3!='':
            x = float (x3)
            y = float (y3)
            #print x," ",y

if(__name__ == "__main__"):
    something = GPSsubscriber()
    something.spin()
