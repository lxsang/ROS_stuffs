#include <ros/ros.h>
#define MLOG ROS_INFO
#ifdef __cplusplus
extern "C"
{
#endif
#include "watchdog.h"

#ifdef __cplusplus
}
#endif

#include "std_msgs/Int32.h"
#include "std_msgs/String.h"
ros::Publisher pub; 

void callback(const std_msgs::Int32::ConstPtr& msg)
{
   	ROS_INFO("I heard: [%d]", msg->data);
	if(send_beacon(9196,"wlp2s0", msg->data))
	{
		std_msgs::String str;
		std::stringstream ss;
		ss << "Sent out beacon "<< msg->data;
		str.data = ss.str();
		ROS_INFO("%s", str.data.c_str());
		pub.publish(str);
	}
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "beacon");
	ros::NodeHandle n;
	pub = n.advertise<std_msgs::String>("beacon", 1000);
	ros::Subscriber sub = n.subscribe<std_msgs::Int32>("/portal", 50,
                                                   &callback);
	ros::Rate loop_rate(10);
	ros::spin();
	return 0;
    
}