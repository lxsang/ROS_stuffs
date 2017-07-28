
#include "Int32Helper.h"


int Int32Helper::hash()
{
    return _H("std_msgs/Int32");
}
void Int32Helper::rawToRosMsg(uint8_t* data)
{
    std_msgs::Int32* ptr = new std_msgs::Int32;
    ptr->data = (int)*data;
    ROS_INFO("Got data %d",ptr->data);
    _msg = (void*) ptr; 
}
int Int32Helper::rosMsgToRaw(uint8_t** data)
{
    
    std_msgs::Int32* ptr = (std_msgs::Int32*)_msg;
    *data = (uint8_t*)malloc(sizeof(int));
    memcpy(*data,&(ptr->data),sizeof(int));
    return len;
}

