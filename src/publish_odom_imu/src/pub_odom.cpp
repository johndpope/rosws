/*
从laser_scan_match读取数据
处理后发布
发布节点名称为odom
*/
#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Pose2D.h>
using namespace std;

ros::Publisher imu_pub;
nav_msgs::Odometry odom_data;

void dataCallback(const geometry_msgs::Pose2D::ConstPtr &msg)
{
    //ROS_INFO("Hello test1_a! I am test1_b. I heard:[%s]",msg->data.c_str());
    imu_pub.publish(odom_data);
}

int main (int argc, char** argv) {
    ros::init(argc, argv, "odom_pub");
    ros::NodeHandle n;

    imu_pub = n.advertise<nav_msgs::Odometry>("odom", 100);
    ros::Subscriber sub = n.subscribe("pose2D", 100, dataCallback);

    ros::spin();

    return 0;
}