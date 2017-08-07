#include <ros/ros.h>
#define MLOG ROS_INFO
#ifdef __cplusplus
extern "C"
{
#endif
#include "bridge/watchdog.h"

#ifdef __cplusplus
}
#endif
#include "helpers/helper.h"

ros::Publisher pub;

void send_newmap(const nav_msgs::OccupancyGrid::ConstPtr& msg)
{
	ROS_INFO("Map available");
	
    OccupancyGridHelper hp;
    
    nav_msgs::OccupancyGrid* m = new nav_msgs::OccupancyGrid(*msg);
    hp.consume((void*)m);
    struct portal_data_t d = hp.getPortalDataFor("");
    d.publish_to = "/other_map";
    d.hash = OccupancyGridHelper::hash();
    ROS_INFO("broadcasting the map");
    upd_data_broadcast(9196,"wlp2s0",d);
    // free allocated memory
    if(d.data) free(d.data);
    //ROS_INFO("Feed x:%f, st.sec:%d, st.nsec:%d, fid:%s", m.seq, m.stamp.sec,m.stamp.nsec,m.frame_id.c_str());
    //pub.publish(m);
}

static int sockfd=-1;
int main(int argc, char **argv)
{
    ros::init(argc, argv, "feed");
	ros::NodeHandle n;
	ros::Subscriber sub1 = n.subscribe<nav_msgs::OccupancyGrid>("/map", 100,&send_newmap);
    ros::spin();
	return 0;
    
}
