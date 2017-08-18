#include <ros/ros.h>
#include "multi_master_bridge/MapData.h"
#include "nav_msgs/OccupancyGrid.h"
#include "geometry_msgs/Pose.h"

#define UNKNOWN -1
std::string other_map_,my_map_,merged_map_topic,map_update_;
bool furious_merge, has_local;
nav_msgs::OccupancyGridPtr global_map;
nav_msgs::OccupancyGridPtr local_map;
geometry_msgs::Pose my_pose;
ros::Publisher  global_map_pub;
ros::Publisher  map_update_pub;
std::map<std::string,  multi_master_bridge::MapData> pipeline;

void getRelativePose(geometry_msgs::Pose p, geometry_msgs::Pose q, geometry_msgs::Pose &delta, double resolution) {
  
    delta.position.x = round((p.position.x - q.position.x) / resolution);
    delta.position.y = round((p.position.y - q.position.y) / resolution);
    delta.position.z = round((p.position.z - q.position.z) / resolution);
}


/*
 * Algorithm 1 - Greedy Merging
 * yz14simpar
 */
 /*
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
}*/
void greedyMerging(int delta_x, int delta_y, int x, int y, const nav_msgs::OccupancyGrid their_map, bool furious) {
  for(int i = 0; i < (int)their_map.info.width; i++) {
    for(int j = 0; j < (int)their_map.info.height; j++) {
        if(i+delta_x + x  >= 0 && i+delta_x + x  < (int)global_map->info.width &&
	        j+delta_y + y  >= 0 && j+delta_y + y  < (int)global_map->info.height) {
            int mycell = i + delta_x + x +(j + delta_y + y)*(int)global_map->info.width;
            int theircell = i + j*(int)their_map.info.width;
            if((int)global_map->data[mycell] == UNKNOWN || ( furious && (int)their_map.data[theircell] != UNKNOWN && (int)global_map->data[mycell] != (int)their_map.data[theircell] ))
            {
                //ROS_INFO("merging...");
            global_map->data[mycell] = their_map.data[theircell];
            }
        }
    }
  }
}

void mege_pipeline(bool furious)
{
    geometry_msgs::Pose delta;
    if ( !local_map ) {
        ROS_INFO("Local map not found, wait for it");
        return;
    }
    std::map<std::string,  multi_master_bridge::MapData>::iterator it;
    global_map.reset(new nav_msgs::OccupancyGrid(*local_map));
    for ( it = pipeline.begin(); it != pipeline.end(); it++ )
    {
        geometry_msgs::Pose p;
        p.position = it->second.position;
        ROS_INFO("mergin map of %s with init pose (%f,%f,%f)",it->first.c_str(), it->second.position.x,it->second.position.y,it->second.position.z);
        ROS_INFO("Get relative position");
        getRelativePose(p,my_pose, delta,global_map->info.resolution);
        ROS_INFO("merging");
        greedyMerging(round(delta.position.x), round(delta.position.y),it->second.x,it->second.y, it->second.map,furious);

    }
    ros::Time now = ros::Time::now();
    global_map->info.map_load_time = now;
    global_map->header.stamp = now;
    ROS_INFO("Publishing global map");
    global_map_pub.publish(*global_map);
}

int get_visible_zone(multi_master_bridge::MapData* data,const nav_msgs::OccupancyGrid::ConstPtr& msg)
{
    int x = -1, y = -1,x1 = -1, y1 = -1, i,j;
    // find the bounding box for the updated zone
    for(i = 0; i <msg->info.width;i++)
        for(j = 0; j < msg->info.height;j++)
        {
            if(msg->data[i+j* msg->info.width]  != UNKNOWN )
            {
                x = i;
                goto findy;
            }
        }
    findy:
    if(x == -1) goto end;
    for(i = 0; i <msg->info.height;i++)
        for(j = 0; j < msg->info.width;j++)
        {
            if(msg->data[j+i* msg->info.width] != UNKNOWN)
            {
                y = i;
                goto findw;
            }
        }
    
    findw:
    for(i = msg->info.width -1; i>=0;i--)
        for(j = 0; j < msg->info.height;j++)
        {
            if(msg->data[i+j* msg->info.width]  != UNKNOWN)
            {
                x1 = i;
                goto findh;
            }
        }
    findh:
     for(i = msg->info.height - 1; i >= 0;i--)
        for(j = 0; j < msg->info.width;j++)
        {
            if(msg->data[j+i* msg->info.width] != UNKNOWN)
            {
                y1 = i;
                goto end;
            }
        }
    end:
    if(x == -1 || y == -1 || x1 == -1 || y1 == -1)
    {
        ROS_INFO("No update found");
        return 0;
    }
    // new map data
    int w = x1 - x;
    int h = y1 - y;
    data->x = x;
    data->y = y;
     ROS_INFO("update zone (%d,%d) (%d,%d)",x, y,w,h);
    data->map.data.resize(w*h,UNKNOWN);
    // copy the update zone to the update map
    for(i=0; i < w;i++)
        for(j = 0; j < h; j++)
            data->map.data[i+j*w] = msg->data[ x + i + (j+y)*msg->info.width ];
    return 1;
}

void register_local_map(const nav_msgs::OccupancyGrid::ConstPtr& msg)
{
    ROS_INFO("Local map found");
    multi_master_bridge::MapData visibledata;
    
    if(get_visible_zone(&visibledata, msg))
    {
        visibledata.position = my_pose.position;
        ROS_INFO("Local update available");
        map_update_pub.publish(visibledata);
        local_map.reset(new nav_msgs::OccupancyGrid(*msg)); 
    }
    else
    {
        ROS_INFO("No local update available");
    }
}
/*
void merge_their_map(const multi_master_bridge::MapData::ConstPtr& msg)
{
    geometry_msgs::Pose p;
    p.position = msg->position;
    ROS_INFO("Merging map from another robot");
    ROS_INFO("Their init pose is [%f,%f,%f]\n",p.position.x,p.position.y, p.position.z);
    ROS_INFO("update zone (%d,%d) (%d,%d)",msg->x, msg->y,msg->map.info.width,msg->map.info.height);
    other_map_pub.publish(msg->map);
    merge_map(p,msg->x, msg->y,msg->map, false);
}*/
void register_neighbor_map(const multi_master_bridge::MapData::ConstPtr& msg)
{
    ROS_INFO("Registering neighbor map");
    pipeline[msg->ip] = *msg;
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "greedy_merging");
	ros::NodeHandle n("~");
    
    n.param<std::string>("other_map",other_map_, "/other_map");
    n.param<std::string>("my_map",my_map_, "/map");
    n.param<std::string>("map_update_topic",map_update_, "/map_update");
    n.param<std::string>("merged_map_topic",merged_map_topic, "/global_map");
    n.param<bool>("furious_merge",furious_merge, false);
    n.param<double>("init_z",my_pose.position.z, 0.0);
	n.param<double>("init_x",my_pose.position.x, 0.0);
	n.param<double>("init_y",my_pose.position.y, 0.0);
    // subscribe to this map
    ROS_INFO("My initial position is [%f,%f,%f]\n",my_pose.position.x,my_pose.position.y, my_pose.position.z);
    ROS_INFO("My Map topic is %s",my_map_.c_str());
    ROS_INFO("Other map topic is %s", other_map_.c_str());
    ROS_INFO("Global map topic is %s", merged_map_topic.c_str());
    ROS_INFO("Furios merge is  %d", furious_merge );
    ros::Subscriber sub = n.subscribe<multi_master_bridge::MapData>(other_map_, 50,&register_neighbor_map);
	ros::Subscriber sub1 = n.subscribe<nav_msgs::OccupancyGrid>(my_map_, 50,&register_local_map);
    global_map = nullptr;
    local_map = nullptr;
    // publisher register
    global_map_pub = n.advertise<nav_msgs::OccupancyGrid>(merged_map_topic, 50, true);
    map_update_pub = n.advertise<multi_master_bridge::MapData>(map_update_, 50, true);
    ros::Rate r(1);
    while(ros::ok())
    {
        ros::spinOnce();
        mege_pipeline(furious_merge);
        r.sleep();
    }
}