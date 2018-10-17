/*
直接从串口读取mpu6050数据
然后将数据处理成为Imu类型数据发布
发布节点名称为imu_data
*/
#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Pose2D.h>
#include <sensor_msgs/Imu.h>
#include <vector>
#include <serial/serial.h>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
using namespace std;

serial::Serial ser; //声明串口对象 

bool odom_used, imu_used;

ros::Publisher odom_pub;
nav_msgs::Odometry odom_data;

ros::Publisher imu_pub;
sensor_msgs::Imu imu_data;

void dataCallback(const geometry_msgs::Pose2D::ConstPtr &msg)
{
    //ROS_INFO_STREAM("dataCallback start");
    if(ser.isOpen()) 
    { 
        if(ser.available()){
            ROS_INFO_STREAM("Serial Port available");
            string result; 
            result = ser.read(ser.available());
            
            vector<string> data;
            boost::split( data, result, boost::is_any_of( ",!" ), boost::token_compress_on);
            ROS_INFO_STREAM("Read: "<<result.c_str());

            imu_data.header.stamp = ros::Time::now();
            imu_data.header.frame_id = "imu";
            imu_data.orientation.x = atof(data[2].c_str());
            imu_data.orientation.y = -atof(data[1].c_str());
            imu_data.orientation.z = -atof(data[0].c_str());
            imu_data.orientation.w = atof(data[3].c_str());
            imu_data.orientation_covariance = {1, 0, 0,
                                               0, 1, 0,
                                               0, 0, 1};

            odom_data.header.stamp = imu_data.header.stamp;
            //odom_data.header.frame_id = "odom";
            odom_data.pose.pose.position.x = msg->x;
            odom_data.pose.pose.position.y = msg->y;
            odom_data.pose.pose.position.z = 0;
            odom_data.pose.pose.orientation = imu_data.orientation;
            odom_data.pose.covariance = {1, 0, 0, 0, 0, 0,
                                         0, 1, 0, 0, 0, 0,
                                         0, 0, 1, 0, 0, 0,
                                         0, 0, 0, 1, 0, 0,
                                         0, 0, 0, 0, 1, 0,
                                         0, 0, 0, 0, 0, 1};
            if(odom_used)
                odom_pub.publish(odom_data);
            if(imu_used)
                imu_pub.publish(imu_data);

        }
    } 
}

int main (int argc, char** argv) {
    ros::init(argc, argv, "odom_imu_pub");
    ros::NodeHandle n;

    string serial_port;
    n.param<std::string>("serial_port", serial_port, "/dev/ttyUSB2");
    n.param<bool>("odom_pub", odom_used, true);
    n.param<bool>("imu_pub", imu_used, true);

    odom_pub = n.advertise<nav_msgs::Odometry>("odom", 100);
    ros::Subscriber sub = n.subscribe("pose2D", 100, dataCallback);

    imu_pub = n.advertise<sensor_msgs::Imu>("imu_data", 100);

    ROS_INFO_STREAM("serial port is:"<<serial_port.c_str());
   
    //连接串口 传输数据
    try 
    { 
    //设置串口属性，并打开串口 
        ser.setPort(serial_port.c_str()); 
        ser.setBaudrate(115200); 
        serial::Timeout to = serial::Timeout::simpleTimeout(1000); 
        ser.setTimeout(to); 
        ser.open(); 
    } 
    catch (serial::IOException& e) 
    { 
        ROS_ERROR_STREAM("Unable to open port "); 
        return -1; 
    } 

    ros::spin();   
}
