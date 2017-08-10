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
#include "multi_master_bridge/NeighbourList.h"
static int sockfd = -1;
int robot_decay_time_s;

void sig_handle(int s)
{
	if(sockfd > 0)
		close(sockfd);
	ROS_INFO("End sniffing");
	ros::shutdown();
}
int listen_to;
std::string listen_interface;
multi_master_bridge::NeighbourList mylist;
int main(int argc, char **argv)
{
    ros::init(argc, argv, "watch_dog",ros::init_options::NoSigintHandler);
	ros::NodeHandle n("~");
	n.param<int>("listen_to", listen_to, 9191);
    n.param<std::string>("listen_interface",listen_interface, "wlan0");
	n.param<int>("robot_decay_time_s",robot_decay_time_s, 5);
	signal(SIGINT, sig_handle);
	mylist.size = 0;
	ros::Publisher dog_pub = n.advertise<multi_master_bridge::NeighbourList>("/new_robot", 1000);
	ros::Rate loop_rate(10);

	sockfd = bind_udp_socket(listen_to);
	struct inet_id_ id = read_inet_id(listen_interface.c_str());
	bool robot_found = false;
	int found_index = -1;
	multi_master_bridge::NeighbourId msg;
	if(sockfd > 0)
	{
		while (ros::ok())
		{
			struct beacon_t a = sniff_beacon(sockfd,id);
			if(a.status)
			{
				// check if the id is exist
				msg.header.stamp = ros::Time::now();
				msg.header.frame_id = std::string(a.hostname);
				msg.ip = std::string(inet_ntoa(a.ip));
				msg.name = std::string(a.hostname);
				msg.port = a.port;
				robot_found = true;
				//mylist.push_back(msg);
			}
			ros::Time t = ros::Time::now();
			for(int i = 0; i < mylist.size;i++)
			{
				// check if decay
				if(t.sec -  mylist.list[i].header.stamp.nsec > robot_decay_time_s)
				{
					mylist.list[i].status = -1; // ofline
					ROS_INFO(" %s become offline, it is no longer available for exchanging", mylist.list[i].name.c_str());
				}
				if(found_index != -1 && robot_found && msg.ip.compare(mylist.list[i].ip) == 0)
				{
					found_index = i;
				}
			}
			if(robot_found)
			{
				if(found_index != -1)
				{
					ROS_INFO("neighbour is existing, update its info");
					mylist.list[found_index].header = msg.header;
					mylist.list[found_index].name = msg.name;
					mylist.list[found_index].port = msg.port;
					mylist.list[found_index].status =0;
					found_index = -1;
				} else 
				{
					mylist.size++;
					msg.status = 1;
					mylist.list.push_back(msg);
				}
				dog_pub.publish(mylist);
				robot_found = false;
			}
			ros::spinOnce();
			loop_rate.sleep();
		}
	}
	return 0;
    
}