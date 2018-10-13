#!/usr/bin/env python
# -*- coding: utf-8 -*-
 
import numpy as np 
import matplotlib.pyplot as plt 
import matplotlib.animation as animation
import rospy
from std_msgs.msg import String

newdata = 0.00
flag = True

def callback(imgmsg):
    global flag
    global newdata
    flag = True
    newdata = float(imgmsg.data.split(",")[0])
    if(newdata > 180):
        newdata = newdata - 360
    rospy.loginfo("newdata is %s", newdata)
    flag = False

num =[0.0 for _ in range(600)]
xaxes = [x for x in range(0, 600)]
fig = plt.figure() 
axes1 = fig.add_subplot(111) 
line, = axes1.plot(xaxes, num) 
axes1.set_ylim(-181, 181) #设置y轴范围
axes1.set_yticks([-180, -170, -160, -150, -140, -130, -120, -110, -100,
                  -90, -80, -70, -60, -50, -40, -30, -20, -10, 0,
                  10, 20, 30, 40, 50, 60, 70, 80, 90, 100,
                  110, 120, 130, 140, 150, 160, 170, 180])
plt.xticks([]) #关闭x轴刻度

def update(data): 
  line.set_ydata(data) 
  return line

def listener():
    while(not rospy.is_shutdown()):
        global flag
        while(flag):
            rospy.loginfo("Not get data!")
        global num
        global newdata
        num = num[1:] + [newdata]
        rospy.loginfo("num is %f", num[-1])
        yield num


ani = animation.FuncAnimation(fig, update, listener, interval=100, repeat=False)
rospy.init_node("listener", anonymous=True)
rospy.Subscriber("read", String, callback)
plt.show()
rospy.spin()
