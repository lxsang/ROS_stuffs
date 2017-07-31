
#include "QuaternionHelper.h"


int QuaternionHelper::hash()
{
    return _H("geometry_msgs/Quaternion");
}
void QuaternionHelper::rawToRosMsg(uint8_t* data)
{
    geometry_msgs::Quaternion* ptr = new geometry_msgs::Quaternion;
    memcpy(&ptr->x, data , sizeof(ptr->x));
    memcpy(&ptr->y, data+sizeof(ptr->x) ,sizeof(ptr->y));
    memcpy(&ptr->z, data + 2*sizeof(ptr->x),sizeof(ptr->z));
    memcpy(&ptr->w, data + 3*sizeof(ptr->x),sizeof(ptr->w));
    _rawsize = 4*sizeof(ptr->x);
    _msg = (void*) ptr; 
}
int QuaternionHelper::rosMsgToRaw(uint8_t** data)
{
    
    geometry_msgs::Quaternion* ptr = (geometry_msgs::Quaternion*)_msg;
    int len=4*sizeof(ptr->x);
    *data = (uint8_t*) malloc(len);
    memcpy(*data,&ptr->x, sizeof(ptr->x));
    memcpy(*data + sizeof(ptr->x),&ptr->y, sizeof(ptr->y));
    memcpy(*data + 2*sizeof(ptr->x),&ptr->z, sizeof(ptr->z));
    memcpy(*data + 3*sizeof(ptr->x),&ptr->w, sizeof(ptr->w));
    return len;
}

