#!/usr/bin/env python
# -*- coding: utf-8 -*-
 
import numpy as np 
import matplotlib.pyplot as plt 
import matplotlib.animation as animation
import rospy
from std_msgs.msg import String

newdata = 0.0
flag = False

def callback(imgmsg):
    global flag
    global newdata
    flag = True
    newdata = 360- float(imgmsg.data.split(",")[2].split("!")[0])
    rospy.loginfo("newdata is %s", newdata)
    flag = False

num =[0.0 for _ in range(60)]
xaxes = [x for x in range(0, 60)]
fig = plt.figure() 
axes1 = fig.add_subplot(111) 
line, = axes1.plot(xaxes, num) 
axes1.set_ylim(-180, 180) #设置y轴范围
plt.xticks([]) #关闭x轴刻度

def update(data): 
  line.set_ydata(data) 
  return line

def listener():
    while(not rospy.is_shutdown()):
        global flag
        while(flag):
            pass
        global num
        global newdata
        num = num[1:] + [newdata]
        rospy.loginfo("num is %d", num[-1])
        yield num


ani = animation.FuncAnimation(fig, update, listener, interval=1000, repeat=False)
rospy.init_node("listener", anonymous=True)
rospy.Subscriber("read", String, callback)
plt.show()
rospy.spin()