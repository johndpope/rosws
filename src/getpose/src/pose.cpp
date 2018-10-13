#include<ros/ros.h>
#include<vector>
#include<geometry_msgs/Pose2D.h>
#include<math.h>
using namespace std;

vector<geometry_msgs::Pose2D> myPose;//储存开始位置和当前位置
bool is_first;

//平方计算
double pow2(double x)
{
  return x*x;
}

void poseCallback(const geometry_msgs::Pose2D& msg)
{
  // 将接收到的消息打印出来
  ROS_INFO("my pose is: (%f, %f, %f).", msg.x, msg.y, msg.theta);

  if(!is_first){
    myPose[1] = msg;
    ROS_INFO("two point distance is:%f.",
              sqrt(pow2(myPose[0].x - myPose[1].x) + pow2(myPose[0].y - myPose[1].y)));
  }else{
    myPose[0] = msg;
  }

  is_first = false;
}

int main(int argc, char **argv)
{
  // 初始化ROS节点
  ros::init(argc, argv, "listener");
  is_first = true;

  // 创建节点句柄
  ros::NodeHandle n;
  myPose.resize(2);

  ros::Subscriber sub = n.subscribe("pose2D", 5, poseCallback);

  ros::spin();

  return 0;
}
