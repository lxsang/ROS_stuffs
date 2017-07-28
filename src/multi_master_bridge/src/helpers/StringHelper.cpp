
#include "StringHelper.h"


int StringHelper::hash()
{
    return _H("std_msgs/String");
}
void StringHelper::rawToRosMsg(uint8_t* data)
{
    ROS_INFO("Converting %d", size);
    std_msgs::String* ptr = new std_msgs::String;
    char* cs = (char*)malloc(size+1);
    memcpy(cs,data,size);
    cs[size] = '\0';
    ptr->data = string(cs);
    ROS_INFO("Got data %s",ptr->data.c_str());
    _msg = (void*) ptr; 
}
int StringHelper::rosMsgToRaw(uint8_t** data)
{
    
    std_msgs::String* ptr = (std_msgs::String*)_msg;
    int len = ptr->data.size();
    *data = (uint8_t*)malloc(len);
    memcpy(*data,ptr->data.c_str(),len);
    return len;
}

