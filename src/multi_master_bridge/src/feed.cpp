#include <ros/ros.h>
#define MLOG ROS_INFO
#ifdef __cplusplus
extern "C"
{
#endif
#include "data_portal.h"

#ifdef __cplusplus
}
#endif
#include "helpers/StringHelper.h"
#include "std_msgs/String.h"
#include "multi_master_bridge/NeighbourId.h"

ros::Publisher pub;

void callback(const multi_master_bridge::NeighbourId::ConstPtr& msg)
{
	std_msgs::String m;
	m.data = string("This is a test data");
	StringHelper hp;
	hp.consume((void*)&m);
    struct portal_data_t d = hp.getPortalDataFor("10.1.16.45");
	d.hash = StringHelper::hash();

	ROS_INFO("Feed %s", m.data.c_str());
	pub.publish(m);
	teleport_raw_data(msg->ip.c_str(),msg->port,d);
}

static int sockfd=-1;
int main(int argc, char **argv)
{
    ros::init(argc, argv, "feed");
	ros::NodeHandle n;
	pub = n.advertise<std_msgs::String>("feed", 1000);
	ros::Subscriber sub = n.subscribe<multi_master_bridge::NeighbourId>("/new_robot", 50,&callback);
	ros::Rate loop_rate(10);
    ros::spin();

	return 0;
    
}