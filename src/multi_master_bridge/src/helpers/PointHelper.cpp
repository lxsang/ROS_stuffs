
#include "PointHelper.h"


int PointHelper::hash()
{
    return _H("geometry_msgs/Point");
}
void PointHelper::rawToRosMsg(uint8_t* data)
{
    geometry_msgs::Point* ptr = new geometry_msgs::Point;
    memcpy(&ptr->x, data , sizeof(ptr->x));
    memcpy(&ptr->y, data+sizeof(ptr->x) ,sizeof(ptr->y));
    memcpy(&ptr->z, data + 2*sizeof(ptr->x),sizeof(ptr->z));
    _rawsize = 3*sizeof(ptr->x);
    _msg = (void*) ptr; 
}
int PointHelper::rosMsgToRaw(uint8_t** data)
{
    
    geometry_msgs::Point* ptr = (geometry_msgs::Point*)_msg;
    int len=3*sizeof(ptr->x);
    *data = (uint8_t*) malloc(len);
    memcpy(*data,&ptr->x, sizeof(ptr->x));
    memcpy(*data + sizeof(ptr->x),&ptr->y, sizeof(ptr->y));
    memcpy(*data + 2*sizeof(ptr->x),&ptr->z, sizeof(ptr->z));
    return len;
}

