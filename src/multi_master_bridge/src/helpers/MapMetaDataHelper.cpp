
#include "MapMetaDataHelper.h"
#include "PoseHelper.h"

int MapMetaDataHelper::hash()
{
    return _H("nav_msgs/MapMetaData");
}
void MapMetaDataHelper::rawToRosMsg(uint8_t* data)
{
    nav_msgs::MapMetaData* ptr = new nav_msgs::MapMetaData;
    PoseHelper pose_h;
    pose_h.consume(data+5*sizeof(int),-1);
    ptr->origin = *((geometry_msgs::Pose*)(pose_h.msg()));

    memcpy(&(ptr->map_load_time.sec),data, sizeof(int));
    memcpy(&(ptr->map_load_time.nsec),data+sizeof(int), sizeof(int));
    memcpy(&(ptr->resolution),data+2*sizeof(int), sizeof(int));
    memcpy(&(ptr->width),data+3*sizeof(int), sizeof(int));
    memcpy(&(ptr->height), data+4*sizeof(int), sizeof(int));
    _rawsize = pose_h.rawsize() + 5*sizeof(int);
    _msg = (void*) ptr; 
}
int MapMetaDataHelper::rosMsgToRaw(uint8_t** data)
{
    
    nav_msgs::MapMetaData* ptr = (nav_msgs::MapMetaData*)_msg;
    int len= 0,plen=0;
    PoseHelper pose_h;
    pose_h.consume((void*)&(ptr->origin));
    uint8_t* pose_raw;
    plen = pose_h.raw(&pose_raw);
    len = 5*sizeof(int)+plen;
    *data = (uint8_t*) malloc(len);
    
    memcpy(*data,&(ptr->map_load_time.sec), sizeof(int));
    memcpy(*data+sizeof(int),&(ptr->map_load_time.nsec), sizeof(int));
    memcpy(*data +2*sizeof(int),&(ptr->resolution), sizeof(int));
    memcpy(*data+3*sizeof(int),&(ptr->width), sizeof(int));
    memcpy(*data+4*sizeof(int),&(ptr->height), sizeof(int));
    memcpy(*data+5*sizeof(int),pose_raw, plen);
    if(pose_raw) free(pose_raw);
    return len;
}

