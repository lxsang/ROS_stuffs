
#include "StringHelper.h"


int StringHelper::hash()
{
    return _H("std_msgs/String");
}
void StringHelper::rawToRosMsg(uint8_t* data)
{
    std_msgs::String* ptr = new std_msgs::String;
    char* cs = (char*)malloc(_rawsize+1);
    memcpy(cs,data,_rawsize);
    cs[_rawsize] = '\0';
    ptr->data = string(cs); 
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

