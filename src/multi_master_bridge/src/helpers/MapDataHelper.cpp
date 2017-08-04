
#include "MapDataHelper.h"
#include "PointHelper.h"
#include "OccupancyGridHelper.h"

int MapDataHelper::hash()
{
    return _H("multi_master_bridge/MapData");
}
void MapDataHelper::rawToRosMsg(uint8_t* data)
{
    multi_master_bridge::MapData* ptr = new multi_master_bridge::MapData;
    //conversion code here 
    PointHelper ph;
    OccupancyGridHelper op;
    ph.consume(data,-1);
    op.consume(data+ph.rawsize(),-1);
    ptr->position = *((geometry_msgs::Point*)ph.msg());
    ptr->map = *((nav_msgs::OccupancyGrid*)op.msg());
    _rawsize = ph.rawsize() + op.rawsize();
    _msg = (void*) ptr; 
}
int MapDataHelper::rosMsgToRaw(uint8_t** data)
{
    
    multi_master_bridge::MapData* ptr = (multi_master_bridge::MapData*)_msg;
    PointHelper ph;
    OccupancyGridHelper oh;
    ph.consume((void*)&(ptr->position));
    oh.consume((void*)&(ptr->map));

    uint8_t* praw,*oraw;
    int plen = ph.raw(&praw);
    int olen = oh.raw(&oraw);
   // ROS_INFO("Length %d %d\n",plen,olen);
    *data = (uint8_t*) malloc(plen+olen);
    memcpy(*data,praw,plen);
    memcpy(*data+plen,oraw,olen);
    //conversion code here
    return plen+olen;
}

