#include <ros/ros.h>
#ifdef __cplusplus
#define MLOG ROS_INFO
extern "C"
{
#endif
#include "watchdog.h"

#ifdef __cplusplus
}
#endif
#include "std_msgs/String.h"
int main(int argc, char **argv)
{
    ros::init(argc, argv, "watch_dog");
	ros::NodeHandle n;
	ros::Publisher dog_pub = n.advertise<std_msgs::String>("new_robot", 1000);
	ros::Rate loop_rate(10);
    //WatchDog dog("wlp2s0",9195);

	while (ros::ok())
	{
        struct in_addr* a = sniff_beacon(9196);
        if(a)
        {
            std_msgs::String msg;
		    std::stringstream ss;
		    ss << " Found new neighbour ";
            ss << inet_ntoa(*a);
		    msg.data = ss.str();
		    ROS_INFO("%s", msg.data.c_str());
		    dog_pub.publish(msg);
        }
		ros::spinOnce();
		loop_rate.sleep();
	}
	return 0;
    
}