#!/usr/bin/env python
# -*- coding: utf-8 -*-
 
import numpy as np 
import matplotlib.pyplot as plt 
import matplotlib.animation as animation
import rospy
from std_msgs.msg import String

newdata = 0.00
flag = True  #表示是否接收到newdata 当值为true时表示没有接收到数据
num =[0.0 for _ in range(600)] #画图数据
data_index = int(input("input num of data:")) - 1

def callback(imgmsg):
    global flag
    global newdata
    global data_index
    flag = True
    newdata = float(imgmsg.data.split(",")[data_index])
    if(newdata > 180):
        newdata = newdata - 360
    rospy.loginfo("newdata is %s", newdata)
    flag = False

xaxes = [x for x in range(0, 600)]
fig = plt.figure() 
axes1 = fig.add_subplot(111) 
line, = axes1.plot(xaxes, num) 
axes1.set_ylim(-181, 181) #设置y轴范围
axes1.set_yticks([-180, -170, -160, -150, -140, -130, -120, -110, -100,
                  -90, -80, -70, -60, -50, -40, -30, -20, -10, 0,
                  10, 20, 30, 40, 50, 60, 70, 80, 90, 100,
                  110, 120, 130, 140, 150, 160, 170, 180])
title = "pic" + str(data_index)
axes1.set_title(title)
axes1.yaxis.grid(color='r', linestyle='--', linewidth=1,alpha=0.3)
plt.xticks([]) #关闭x轴刻度

def update(data): 
  line.set_ydata(data) 
  return line

def listener():
    while(not rospy.is_shutdown()):
        global flag
        while(not flag):
            rospy.loginfo("Not get data!")
        global num
        global newdata
        num = num[1:] + [newdata]
        rospy.loginfo("num is %f", num[-1])
        yield num  #利用迭代器返回数据传给update函数

ani = animation.FuncAnimation(fig, update, listener, interval=100, repeat=False)
rospy.init_node("listener", anonymous=True)
rospy.Subscriber("read", String, callback)
plt.show()
rospy.spin()
