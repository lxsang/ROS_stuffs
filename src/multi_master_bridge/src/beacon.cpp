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
int broadcast_to,broadcast_rate;
std::string broadcast_interface;
int port = -1;
void callback(const std_msgs::Int32::ConstPtr& msg)
{
	port = msg->data;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "beacon");
	ros::NodeHandle n;
	n.param<int>("/beacon/broadcast_to", broadcast_to, 9196);
    n.param<std::string>("/beacon/broadcast_interface",broadcast_interface, "wlan0");
	n.param<int>("/beacon/refresh_rate",broadcast_rate, 10);
	pub = n.advertise<std_msgs::String>("beacon", 1000);
	ros::Subscriber sub = n.subscribe<std_msgs::Int32>("/portal", 50,
                                                   &callback);
	port = -1;
	ros::Rate loop_rate(broadcast_rate);
	while(ros::ok())
	{
		ros::spinOnce();
		if(port == -1) continue;
		if(send_beacon(broadcast_to,broadcast_interface.c_str(),port))
		{
			std_msgs::String str;
			std::stringstream ss;
			ss << "Sent out beacon "<< port;
			ss << " on "<< broadcast_to;
			str.data = ss.str();
			ROS_INFO("%s", str.data.c_str());
			pub.publish(str);
		}
		loop_rate.sleep();
	}
	return 0;
    
}