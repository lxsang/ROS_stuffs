#include <ros/ros.h>
#include "multi_master_bridge/MapData.h"
#include "nav_msgs/OccupancyGrid.h"
#include "geometry_msgs/Pose.h"

#define UNKNOWN -1

std::string other_map_,my_map_,merged_map_topic;
nav_msgs::OccupancyGridPtr global_map;
geometry_msgs::Pose my_pose;
ros::Publisher  global_map_pub;
ros::Publisher  other_map_pub;

void getRelativePose(geometry_msgs::Pose p, geometry_msgs::Pose q, geometry_msgs::Pose &delta, double resolution) {
  
    delta.position.x = round((p.position.x - q.position.x) / resolution);
    delta.position.y = round((p.position.y - q.position.y) / resolution);
    delta.position.z = round((p.position.z - q.position.z) / resolution);
}


/*
 * Algorithm 1 - Greedy Merging
 * yz14simpar
 */
void greedyMerging(int delta_x, int delta_y, const nav_msgs::OccupancyGrid their_map) {
  int offset_w, offset_h;
  offset_h = ((int)global_map->info.height - (int)their_map.info.height);
  offset_w = ((int)global_map->info.width - (int)their_map.info.width);
  ROS_INFO("global (%d,%d) their (%d,%d)",global_map->info.width, global_map->info.height, their_map.info.width, their_map.info.height);
  ROS_INFO("Offset w %d offset h %d", offset_w, offset_h);
  for(int i = 0; i < (int)global_map->info.width; i++) {
    for(int j = 0; j < (int)global_map->info.height; j++) {
      if(i+delta_x - offset_w >= 0 && i+delta_x - offset_w < (int)their_map.info.width &&
	 j+delta_y - offset_h >= 0 && j+delta_y - offset_h < (int)their_map.info.height) {
	if((int)global_map->data[i+j*(int)global_map->info.width] == UNKNOWN)
    {
        //ROS_INFO("merging...");
	  global_map->data[i+j*(int)global_map->info.width] = their_map.data[i+delta_x - offset_w +(j+delta_y - offset_h)*(int)their_map.info.width];
    }
    }
    }
  }
}

void merge_map( geometry_msgs::Pose p,  nav_msgs::OccupancyGrid msg)
{
    if(!global_map)
    {
        ROS_INFO("Global map not found, init it as the provide map");
        global_map.reset(new nav_msgs::OccupancyGrid(msg));
    
    }
    else
    {

        // merge two map here
        geometry_msgs::Pose delta;
        ROS_INFO("Get relative position");
        getRelativePose(my_pose,p, delta,global_map->info.resolution);
        ROS_INFO("merging");
        greedyMerging(round(delta.position.x), round(delta.position.y), msg);
    }
    //global_map->header.frame_id = "map";
    ros::Time now = ros::Time::now();
    global_map->info.map_load_time = now;
    global_map->header.stamp = now;
    ROS_INFO("Publishing global map");
    global_map_pub.publish(*global_map);
}
void merge_local_map(const nav_msgs::OccupancyGrid::ConstPtr& msg)
{

    ROS_INFO("Merging local map");
    //merge_map(my_pose,*msg);
    if(!global_map)
    {
        ROS_INFO("Global map not found, init it as the provide map");
        global_map.reset(new nav_msgs::OccupancyGrid(*msg));
    
    }
}
void merge_their_map(const multi_master_bridge::MapData::ConstPtr& msg)
{
    geometry_msgs::Pose p;
    p.position = msg->position;
    ROS_INFO("Merging map from another robot");
    ROS_INFO("Their init pose is [%f,%f,%f]\n",p.position.x,p.position.y, p.position.z);
    other_map_pub.publish(msg->map);
    merge_map(p,msg->map);
}
int main(int argc, char** argv)
{
    ros::init(argc, argv, "greedy_merging");
	ros::NodeHandle n("~");
    n.param<std::string>("other_map",other_map_, "/other_map");
    n.param<std::string>("my_map",my_map_, "/map");
     n.param<std::string>("merged_map_topic",merged_map_topic, "/global_map");
    n.param<double>("/map_exchange/init_z",my_pose.position.z, 0.0);
	n.param<double>("/map_exchange/init_x",my_pose.position.x, 0.0);
	n.param<double>("/map_exchange/init_y",my_pose.position.y, 0.0);
    // subscribe to this map
    ROS_INFO("My initial position is [%f,%f,%f]\n",my_pose.position.x,my_pose.position.y, my_pose.position.z);
    ROS_INFO("My Map topic is %s",my_map_.c_str());
    ROS_INFO("Other map topic is %s", other_map_.c_str());
    ROS_INFO("Global map topic is %s", merged_map_topic.c_str());
    ros::Subscriber sub = n.subscribe<multi_master_bridge::MapData>(other_map_, 50,&merge_their_map);
	ros::Subscriber sub1 = n.subscribe<nav_msgs::OccupancyGrid>(my_map_, 50,&merge_local_map);
    global_map = nullptr;
    // publisher register
    global_map_pub = n.advertise<nav_msgs::OccupancyGrid>(my_map_, 50, true);
     other_map_pub = n.advertise<nav_msgs::OccupancyGrid>("/map_other", 50, true);
     ros::spin();
}