/*
直接从串口读取mpu6050数据
然后将数据处理成为Imu类型数据发布
发布节点名称为imu_data
*/
#include <ros/ros.h>
#include <sensor_msgs/Imu.h>
#include <vector>
#include <serial/serial.h>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
using namespace std;

int main (int argc, char** argv) {
    ros::init(argc, argv, "imu_pub");
    ros::NodeHandle n;

    ros::Publisher imu_pub;
    imu_pub = n.advertise<sensor_msgs::Imu>("imu_data", 100);

    sensor_msgs::Imu imu_data;

    serial::Serial ser; //声明串口对象 

    
    //连接串口 传输数据
    try 
    { 
    //设置串口属性，并打开串口 
        ser.setPort("/dev/ttyUSB0"); 
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

    //检测串口是否已经打开，并给出提示信息 
    if(ser.isOpen()) 
    { 
        ROS_INFO_STREAM("Serial Port initialized"); 
    } 
    else 
    { 
        return -1; 
    } 

    //指定循环的频率 
    ros::Rate loop_rate(10); 
    while(ros::ok()) 
    {      
        if(ser.available()){
            string result; 
            result = ser.read(ser.available());
            
            vector<string> data;
            boost::split( data, result, boost::is_any_of( ",!" ), boost::token_compress_on);
            ROS_INFO_STREAM("Read: "<<result.c_str());

            imu_data.header.stamp = ros::Time::now();
            imu_data.header.frame_id = "base_link";
            imu_data.orientation.x = atof(data[2].c_str());
            imu_data.orientation.y = -atof(data[1].c_str());
            imu_data.orientation.z = -atof(data[0].c_str());
            imu_data.orientation.w = atof(data[3].c_str());

             imu_pub.publish(imu_data);
        }
         
        //处理ROS的信息，比如订阅消息,并调用回调函数 
        ros::spinOnce(); 
        loop_rate.sleep(); 
    } 
    
}
