#include <combine_grids/merging_pipeline.h>
#include <geometry_msgs/Pose.h>
#include <map_msgs/OccupancyGridUpdate.h>
#include <nav_msgs/OccupancyGrid.h>
#include <ros/ros.h>


class TwinMerge {
    public: 
        TwinMerge();
        void startMerging();
    private: 
        std::string robot_map_topic;
        std::string other_robot_map_topic;
        std::string merged_map_topic;
        ros::NodeHandle node;
        nav_msgs::OccupancyGridPtr global_map;
        ros::Publisher  global_map_pub;
        ros::Subscriber my_map_sub;
        ros::Subscriber their_map_sub;
        void mergeCallBack(const  nav_msgs::OccupancyGrid::ConstPtr& map);
        //void theirMapCallBack(const  nav_msgs::OccupancyGrid::ConstPtr& map);
};

TwinMerge::TwinMerge()
{
    node.param<std::string>("robot_map_topic", robot_map_topic, "/map");
    node.param<std::string>("other_robot_map_topic",other_robot_map_topic, "/map_other");
    node.param<std::string>("merged_map_topic", merged_map_topic, "/map_global");
    global_map = nullptr;
    // publisher register
    global_map_pub = node.advertise<nav_msgs::OccupancyGrid>(merged_map_topic, 50, true);
    // subscriber to the two toic
    ROS_INFO("Subscribing to %s...",robot_map_topic.c_str());
    my_map_sub = node.subscribe<nav_msgs::OccupancyGrid>(robot_map_topic, 1000,&TwinMerge::mergeCallBack, this);
    ROS_INFO("Subscribing to %s...",other_robot_map_topic.c_str());
    their_map_sub = node.subscribe<nav_msgs::OccupancyGrid>(other_robot_map_topic, 1000,&TwinMerge::mergeCallBack, this);
}

void TwinMerge::startMerging()
{
    /*ros::Subscriber my_map_sub;
    ros::Subscriber their_map_sub;
    // subscriber to the two toic
    ROS_INFO("Subscribing to %s...",robot_map_topic.c_str());
    my_map_sub = node.subscribe<nav_msgs::OccupancyGrid>(robot_map_topic, 1000,&TwinMerge::mergeCallBack, this);
    ROS_INFO("Subscribing to %s...",other_robot_map_topic.c_str());
    their_map_sub = node.subscribe<nav_msgs::OccupancyGrid>(other_robot_map_topic, 1000,&TwinMerge::mergeCallBack, this);*/
}

void TwinMerge::mergeCallBack(const  nav_msgs::OccupancyGrid::ConstPtr& map)
{
    // merge my map with global map
    std::vector<nav_msgs::OccupancyGridConstPtr> grids;
    ROS_INFO("Star merging  map with global map");
    combine_grids::MergingPipeline merger;
    if(!global_map)
    {
        ROS_INFO("Global map is not available. Init it as local map");
         global_map.reset(new nav_msgs::OccupancyGrid(*map));
         return;
    }
    nav_msgs::OccupancyGridPtr their_map{new nav_msgs::OccupancyGrid(*map)};
    grids.push_back(global_map);
    grids.push_back(their_map);
    //std::vector<nav_msgs::OccupancyGridConstPtr> maps;
    ROS_INFO("Feeding data");
    merger.feed(grids.begin(),grids.end());
    ROS_INFO("Estimation of transfo");
    merger.estimateTransforms();
    ROS_INFO("Compose new map");
    global_map = merger.composeGrids();
    ROS_INFO("Publishing global map");
    global_map_pub.publish(global_map);
}
int main(int argc, char **argv)
{
    ros::init(argc, argv, "twin_merger");
    TwinMerge merger;
    merger.startMerging();
    ros::spin();
}