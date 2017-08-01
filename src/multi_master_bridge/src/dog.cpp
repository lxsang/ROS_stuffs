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
 #include <signal.h>
#include "multi_master_bridge/NeighbourId.h"
static int sockfd = -1;


void sig_handle(int s)
{
	if(sockfd > 0)
		close(sockfd);
	ROS_INFO("End sniffing");
	ros::shutdown();
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "watch_dog",ros::init_options::NoSigintHandler);
	signal(SIGINT, sig_handle);
	ros::NodeHandle n;
	ros::Publisher dog_pub = n.advertise<multi_master_bridge::NeighbourId>("new_robot", 1000);
	ros::Rate loop_rate(10);

	sockfd = bind_udp_socket(9196);
	struct inet_id_ id = read_inet_id("wlp2s0");
	if(sockfd > 0)
	{
		while (ros::ok())
		{
			struct beacon_t a = sniff_beacon(sockfd,id);
			if(a.status)
			{
				multi_master_bridge::NeighbourId msg;
				std::stringstream ss;
				msg.header.stamp = ros::Time::now();
				msg.header.frame_id = std::string(a.hostname);
				msg.ip = inet_ntoa(a.ip);
				msg.name = std::string(a.hostname);
				msg.port = a.port;
				ss << " Found new neighbour at ";
				ss << msg.ip;
				ss << " name " <<msg.name;
				ss << " listen on:" << msg.port;
				//msg.data = ss.str();
				ROS_INFO("%s", ss.str().c_str());
				dog_pub.publish(msg);
			}
			ros::spinOnce();
			loop_rate.sleep();
		}
	}
	return 0;
    
}