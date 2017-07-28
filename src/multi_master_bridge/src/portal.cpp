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
#include <signal.h>
#include "std_msgs/Int32.h"
#include "helpers/StringHelper.h"
static int sockfd=-1;

ros::Publisher dpub;

void callback(struct portal_data_t d)
{
	//ROS_INFO("sh: %d",simple_hash("std_msgs/String"));
	if(StringHelper::hash() == d.hash)
	{
		
    	ROS_INFO("[%d] Data From: %s %d\n",d.status,d.from,d.size);
		StringHelper hp;
		hp.consume(d.data,d.size);
		dpub.publish(*((std_msgs::String*)hp.msg()));
	} else
	{
		ROS_INFO("Wrong message type, expect %d, but %d",StringHelper::hash(),d.hash);
	}
}
void sig_handle(int s)
{
	if(sockfd > 0)
		close(sockfd);
    //sockfd = -1;
	ROS_INFO("End listener");
	ros::shutdown();
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "portal",ros::init_options::NoSigintHandler);
	ros::NodeHandle n;
	ros::Publisher pub = n.advertise<std_msgs::Int32>("portal", 1000);
	dpub = n.advertise<std_msgs::String>("portal_data", 1000);
	ros::Rate loop_rate(10);
    signal(SIGINT, sig_handle);
    
    unsigned port = 0;
    sockfd = portal_startup(&port);
	std::stringstream ss;
	ss << "Listen on port"<< port;
	std_msgs::Int32 msg;
    msg.data = (int)port;
	ROS_INFO("%s", ss.str().c_str());
	pub.publish(msg);

	int client_sock;
    pthread_t newthread;
    
    while(ros::ok())
    {
		pub.publish(msg);
        client_sock = portal_listen(sockfd);
        if (client_sock == -1)
        {
            continue;
        }
        struct portal_callback_t callback_data;
        callback_data.client = client_sock;
        callback_data.callback = callback;
        if (pthread_create(&newthread , NULL,(void *(*)(void *))portal_checkin, (void *)&callback_data) != 0)
            perror("pthread_create");
        else
        {
            //reclaim the stack data when thread finish
            pthread_detach(newthread) ;
        }
        //portal_checkin(&callback_data);
    }

	return 0;
    
}