
#include "HeaderHelper.h"


int HeaderHelper::hash()
{
    return _H("std_msgs/Header");
}
void HeaderHelper::rawToRosMsg(uint8_t* data)
{
    std_msgs::Header* ptr = new std_msgs::Header;
    //conversion code here
    _msg = (void*) ptr; 
}
int HeaderHelper::rosMsgToRaw(uint8_t** data)
{
    
    std_msgs::Header* ptr = (std_msgs::Header*)_msg;
    int len;
    //conversion code here
    return len;
}

