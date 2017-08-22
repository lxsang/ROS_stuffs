
#include "ros/ros.h"
#include "stdio.h"
#include "math.h"
#include "nav_msgs/OccupancyGrid.h"

std::string map_in_topic_, map_out_topic_;
int th_value_;
double map_resolution_,map_width_m_,map_height_m_;
ros::Publisher  map_pub;
nav_msgs::OccupancyGrid th_map;
void map_threshold(const nav_msgs::OccupancyGrid::ConstPtr& msg)
{
    
    th_map.header = msg->header;
    th_map.info.map_load_time = msg->info.map_load_time;
    // now calculate the transformation of the maps
    int off_x = abs(ceil((th_map.info.origin.position.x - msg->info.origin.position.x)/th_map.info.resolution));
    int off_y = abs(ceil((th_map.info.origin.position.y - msg->info.origin.position.y)/th_map.info.resolution));
    std::fill(th_map.data.begin(), th_map.data.end(), -1);
    //ROS_INFO("offset is (%d, %d) for height (%d, %d)", off_x, off_y, th_map.info.height, msg->info.height);
    for(int i= 0;i < msg->info.width ; i++)
        for(int j = 0; j < msg->info.height; j ++)
        {
             //th_map.data[(off_y  +  j)*th_map.info.width + off_x + i] = 100;
            if(msg->data[i + j*msg->info.width] != -1)
                th_map.data[(off_y +  j)*th_map.info.width + off_x + i] = msg->data[i + j*msg->info.width] > th_value_?100:0;
        }
       
    map_pub.publish(th_map);
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "map_threshold");
	ros::NodeHandle n("~");
    n.param<int>("th_value",th_value_, 50);
    n.param<double>("map_width_m",map_width_m_, 10.0);
    n.param<double>("map_height_m",map_height_m_, 10.0);
    n.param<double>("map_resolution",map_resolution_, 0.05);
    n.param<std::string>("map_in_topic",map_in_topic_, "/map");
    n.param<std::string>("map_out_topic",map_out_topic_, "/threshold_map");
    int w =  ceil((map_width_m_ - 1)/map_resolution_);
    int h = ceil((map_height_m_ - 1)/map_resolution_);
    th_map.data.resize(w*h,-1);
    //ROS_INFO("map size (%d, %d)", w, h);
    th_map.info.width = w;
    th_map.info.height = h;
    th_map.info.resolution = map_resolution_;
    th_map.info.origin.position.x = -map_width_m_/2.0;
    th_map.info.origin.position.y = -map_height_m_/2.0;
    th_map.info.origin.position.z = 0.0;
    th_map.info.origin.orientation.x = 0.0;
    th_map.info.origin.orientation.y = 0.0;
    th_map.info.origin.orientation.z = 0.0;
    th_map.info.origin.orientation.w = 1.0;
    
    ros::Subscriber sub = n.subscribe<nav_msgs::OccupancyGrid>(map_in_topic_, 50,&map_threshold);
    map_pub = n.advertise<nav_msgs::OccupancyGrid>(map_out_topic_, 50, true);
    ros::spin();
}