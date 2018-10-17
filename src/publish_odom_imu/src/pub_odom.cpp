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
    odom_data.header.stamp = ros::Time::now();
    odom_data.header.frame_id = "odom";
    odom_data.pose.pose.position.x = msg->x;
    odom_data.pose.pose.position.y = msg->y;
    odom_data.pose.pose.position.z = 0;
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