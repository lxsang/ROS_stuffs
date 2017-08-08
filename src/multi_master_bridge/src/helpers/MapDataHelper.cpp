
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
    memcpy(&ptr->x,data + ph.rawsize(), sizeof(int));
    memcpy(&ptr->y, data + ph.rawsize() + sizeof(int), sizeof(int));
    op.consume(data+ ph.rawsize() + 2*sizeof(int),-1);
    ptr->position = *((geometry_msgs::Point*)ph.msg());
    ptr->map = *((nav_msgs::OccupancyGrid*)op.msg());
    _rawsize = ph.rawsize() + op.rawsize() + 2*sizeof(int);
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
    *data = (uint8_t*) malloc(plen+olen + 2*sizeof(int));
    memcpy(*data,praw,plen);
    memcpy(*data + plen, &ptr->x,sizeof(int));
    memcpy(*data + plen + sizeof(int), &ptr->y, sizeof(int));
    memcpy(*data + plen + 2*sizeof(int),oraw,olen);
    //conversion code here
    return plen + olen + 2*sizeof(int);
}

