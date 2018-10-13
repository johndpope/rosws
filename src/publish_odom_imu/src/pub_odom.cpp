#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>

int main(){
    
}

// ros::Publisher chatter_pub;
// ros::Subscriber sub;

// void callback(const std_msgs::StringPtr &msg)
// {
//     std::stringstream ss;
//     msg.data = "test";

//     ROS_INFO("%s", msg.data.c_str());

//     chatter_pub.publish(msg);
// }


// int main(int argc, char **argv)
// {

//     ros::init(argc, argv, "odom_pub");

// 	ros::NodeHandle n;
	
//     chatter_pub = n.advertise<std_msgs::String>("odom", 1000);
// 	sub = n.subscribe("/test", 1, &callback);

//     ros::Rate loop_rate(10);


// 	ROS_INFO("Please send message!");
	
// 	ros::spin();
	
//     return 0;
// }
