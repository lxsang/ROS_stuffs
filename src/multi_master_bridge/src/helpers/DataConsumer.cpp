#include "DataConsumer.h"
template <class T1, class T2>  
void Consumer<T1,T2>::consume(ros::Publisher** pub,portal_data_t* pd)
{
    if(!*pub)
    {
        ros::NodeHandle n;
        *pub = new ros::Publisher();
        *(*pub) = n.advertise<T1>(pd->publish_to, 50,true);
    }
    // consume data
    T2 helper;
    helper.consume(pd->data, pd->size);
    helper.template publish<T1>(*pub);
}

void DataConsumer::consume(ros::Publisher** pub,portal_data_t* pd)
{
   
    if(pd->hash == StringHelper::hash())
        INPUT<std_msgs::String,StringHelper>(pub,pd);

    else  if(pd->hash == Int32Helper::hash())
        INPUT<std_msgs::Int32,Int32Helper>(pub,pd);

    else  if(pd->hash == HeaderHelper::hash())
        INPUT<std_msgs::Header,HeaderHelper>(pub,pd);

    else if(pd->hash == PointHelper::hash())
        INPUT<geometry_msgs::Point,PointHelper>(pub,pd);
    
    else if(pd->hash == QuaternionHelper::hash())
        INPUT<geometry_msgs::Quaternion,QuaternionHelper>(pub,pd);

    else if(pd->hash == PoseHelper::hash())
        INPUT<geometry_msgs::Pose,PoseHelper>(pub,pd);

    else if(pd->hash == MapMetaDataHelper::hash())
        INPUT<nav_msgs::MapMetaData,MapMetaDataHelper>(pub,pd);
    
    else if(pd->hash == OccupancyGridHelper::hash())
        INPUT<nav_msgs::OccupancyGrid,OccupancyGridHelper>(pub,pd);

    else
        ROS_INFO("Cannot find data handle for hash %d", pd->hash);
    
}