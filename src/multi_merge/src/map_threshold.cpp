
#include "ros/ros.h"
#include "stdio.h"
#include "math.h"
#include "nav_msgs/OccupancyGrid.h"

std::string map_in_topic_, map_out_topic_;
int th_value_;
ros::Publisher  map_pub;
nav_msgs::OccupancyGrid th_map;
void map_threshold(const nav_msgs::OccupancyGrid::ConstPtr& msg)
{
    
    th_map.header = msg->header;
    th_map.info = msg->info;
    int w =  msg->info.width;
    int h = msg->info.height;
    th_map.data.resize(w*h,-1);
    std::fill(th_map.data.begin(), th_map.data.end(), -1);
    for(int i= 0;i < w*h ; i++)
            if(msg->data[i] != -1)
                th_map.data[i] = msg->data[i] > th_value_?100:0;
       
    map_pub.publish(th_map);
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "map_threshold");
	ros::NodeHandle n("~");
    n.param<int>("th_value",th_value_, 50);
    n.param<std::string>("map_in_topic",map_in_topic_, "/map");
    n.param<std::string>("map_out_topic",map_out_topic_, "/threshold_map");
    
    //ROS_INFO("map size (%d, %d)", w, h);
    
    ros::Subscriber sub = n.subscribe<nav_msgs::OccupancyGrid>(map_in_topic_, 50,&map_threshold);
    map_pub = n.advertise<nav_msgs::OccupancyGrid>(map_out_topic_, 50, true);
    ros::spin();
}