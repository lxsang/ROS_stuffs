
#include "TestHelper.h"


int TestHelper::hash()
{
    return _H("std_msgs/Test");
}
void TestHelper::rawToRosMsg(uint8_t* data)
{
    std_msgs::Test* ptr = new std_msgs::Test;
    //conversion code here
    _msg = (void*) ptr; 
}
int TestHelper::rosMsgToRaw(uint8_t** data)
{
    
    std_msgs::Test* ptr = (std_msgs::Test*)_msg;
    int len;
    //conversion code here
    return len;
}

