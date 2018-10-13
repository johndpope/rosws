#include<ros/ros.h>
#include<std_msgs/String.h>
#include<iostream>
#include<string>
#include <sstream>

int main(int argc, char *argv[])
{

    //初始化ROS
    ros::init(argc, argv, "publish_voice");
    ros::NodeHandle n;
    ros::Rate loop_rate(10);

    ros::Publisher voicePub = n.advertise<std_msgs::String>("voiceWords",1000);
    
    printf("ctrl+z停止输入，退出程序");
    printf("请输入要转换的文字：");
    std_msgs::String msg;
    std::stringstream ss;
    while(std::cin>>msg.data){
         voicePub.publish(msg);
         printf("请输入要转换的文字：");
    }

    exit:
        printf("检测到输入Ctrl+D,程序退出\n");
    

    return 0;
}