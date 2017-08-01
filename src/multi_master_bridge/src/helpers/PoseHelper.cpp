
#include "PoseHelper.h"
#include "PointHelper.h"
#include "QuaternionHelper.h"

int PoseHelper::hash()
{
    return _H("geometry_msgs/Pose");
}
void PoseHelper::rawToRosMsg(uint8_t* data)
{
    geometry_msgs::Pose* ptr = new geometry_msgs::Pose;
    // composite type, use child helpers to consume and produce data
    PointHelper point_h;
    QuaternionHelper quat_h;
    point_h.consume(data,-1);
    quat_h.consume(data+ 3*sizeof(ptr->position.x),-1);
    ptr->position = *((geometry_msgs::Point*)point_h.msg());
    ptr->orientation = *((geometry_msgs::Quaternion*)quat_h.msg());
    _rawsize = point_h.rawsize() + quat_h.rawsize();
    _msg = (void*) ptr; 
}
int PoseHelper::rosMsgToRaw(uint8_t** data)
{
    // composite type, use other helpers to consume and to produce data
    geometry_msgs::Pose* ptr = (geometry_msgs::Pose*)_msg;
    PointHelper point_h;
    QuaternionHelper quat_h;
    point_h.consume((void*)&(ptr->position));
    quat_h.consume((void*)&(ptr->orientation));
    //get raw data
    uint8_t *point_raw, *quat_raw;
    int plen=0,qlen=0;
    plen = point_h.raw(&point_raw);
    qlen = quat_h.raw(&quat_raw);
    int len = plen + qlen;
    *data = (uint8_t*)malloc(len);
    memcpy(*data,point_raw,plen);
    memcpy(*data + plen,quat_raw,qlen);
    if(point_raw) free(point_raw);
    if(quat_raw) free(quat_raw);
    return len;
}

