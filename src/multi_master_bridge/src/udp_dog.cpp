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
#include "helpers/helper.h"
#include "helpers/DataConsumer.h"
static int sockfd = -1;

void sig_handle(int s)
{
	if(sockfd > 0)
		close(sockfd);
	ROS_INFO("End sniffing");
	ros::shutdown();
	exit(0);
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "watch_dog",ros::init_options::NoSigintHandler);
	signal(SIGINT, sig_handle);
	ros::NodeHandle n;
	ros::Rate loop_rate(10);

	sockfd = bind_udp_socket(9196);
	struct inet_id_ id = read_inet_id("wlp2s0");
    ros::Publisher* dpub = NULL;
	if(sockfd > 0)
	{
		while (ros::ok())
		{
			struct portal_data_t d = udp_portal_checkin(sockfd,id);
			if(d.status)
			{
	            DataConsumer consumer;
	            consumer.consume(&dpub,&d);
	            ROS_INFO("[%s] Publish  %d bytes data to:%s\n",d.from,d.size,d.publish_to);
			}
			//ros::spinOnce();
			loop_rate.sleep();
		}
	}
	return 0;
    
}