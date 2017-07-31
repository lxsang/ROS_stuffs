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
#include "helpers/helper.h"
#include "multi_master_bridge/NeighbourId.h"

ros::Publisher pub;

void callback(const multi_master_bridge::NeighbourId::ConstPtr& msg)
{
	nav_msgs::OccupancyGrid m;
	m.header.seq = 10;
	m.header.stamp = ros::Time::now();
	m.header.frame_id = "feeder";
	m.info.map_load_time = ros::Time::now();
	m.info.resolution = 0.1;
	m.info.width = 480;
	m.info.height = 320;
	m.info.origin.position.x = 1.2;
	m.info.origin.position.y = 1.4;
	m.info.origin.position.z = 1.7;
	m.info.origin.orientation.x = 0.7;
	m.info.origin.orientation.y = 0.45;
	m.info.origin.orientation.z = 40.56;
	m.info.origin.orientation.w = 45.6666;
	int i=0;
	for(i;i<255;i++) m.data.push_back(i+1);
	OccupancyGridHelper hp;
	hp.consume((void*)&m);
    struct portal_data_t d = hp.getPortalDataFor("10.1.16.45");
	d.publish_to = "/food_chain";
	d.hash = OccupancyGridHelper::hash();

	//ROS_INFO("Feed x:%f, st.sec:%d, st.nsec:%d, fid:%s", m.seq, m.stamp.sec,m.stamp.nsec,m.frame_id.c_str());
	pub.publish(m);
	teleport_raw_data(msg->ip.c_str(),msg->port,d);
	
	// free allocated memory
	if(d.data) free(d.data);
}

static int sockfd=-1;
int main(int argc, char **argv)
{
    ros::init(argc, argv, "feed");
	ros::NodeHandle n;
	pub = n.advertise<nav_msgs::OccupancyGrid>("feed", 1000);
	ros::Subscriber sub = n.subscribe<multi_master_bridge::NeighbourId>("/new_robot", 50,&callback);
	ros::Rate loop_rate(10);
    ros::spin();

	return 0;
    
}