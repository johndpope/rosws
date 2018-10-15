#include <ros/ros.h> 
#include <serial/serial.h>
#include <std_msgs/String.h> 
#include <fstream>
#include <vector>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
using namespace std;

serial::Serial ser; //声明串口对象 

int main (int argc, char** argv) 
{ 
    vector<string> data;

    fstream yawData;
    fstream pitchData;
    fstream rollData;
    //初始化节点 
    ros::init(argc, argv, "serial_node"); 
    //声明节点句柄 
    ros::NodeHandle nh; 

    //发布主题 
    ros::Publisher read_pub = nh.advertise<std_msgs::String>("read", 1000); 

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
        string yaw, pitch, roll;
        
        if(ser.available()){
            //ROS_INFO_STREAM("Reading from serial port");

            std_msgs::String result; 
            result.data = ser.read(ser.available());
            boost::split( data, result.data, boost::is_any_of( ",!" ), boost::token_compress_on );
            //ROS_INFO_STREAM("Read: "<<result.data);

             pitch = data[1];
             yaw   = data[2];
             roll  = data[0];

            ROS_INFO_STREAM("pitch yaw roll is:"<<pitch<<"  "<<yaw<<"  "<<roll);

            // yawData.open("/home/wode/yaw.txt", ios::out | ios::app);
            // pitchData.open("/home/wode/pitch.txt", ios::in | ios::app);
            // rollData.open("/home/wode/roll.txt", ios::in | ios::app); 

            // yawData << yaw<< "\n";
            // pitchData << pitch << "\n";
            // rollData <<roll<< "\n";

             read_pub.publish(result);

            // yawData.close();
            // pitchData.close();
            // rollData.close(); 
        }
         

        //处理ROS的信息，比如订阅消息,并调用回调函数 
        ros::spinOnce(); 
        loop_rate.sleep(); 
    } 
}