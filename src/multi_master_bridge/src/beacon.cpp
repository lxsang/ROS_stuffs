#include <ros/ros.h>
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

#include "std_msgs/String.h"
int main(int argc, char **argv)
{
    ros::init(argc, argv, "beacon");
	ros::NodeHandle n;
	ros::Publisher dog_pub = n.advertise<std_msgs::String>("becon", 1000);
	ros::Rate loop_rate(10);
    //WatchDog dog("wlp2s0",9195);

	while (ros::ok())
	{
        if(send_beacon(9196,"wlp2s0"))
        {
            std_msgs::String msg;
		    std::stringstream ss;
		    ss << "Sent out beacon";
		    msg.data = ss.str();
		    ROS_INFO("%s", msg.data.c_str());
		    dog_pub.publish(msg);
        }
		ros::spinOnce();
		loop_rate.sleep();
	}
	return 0;
    
}