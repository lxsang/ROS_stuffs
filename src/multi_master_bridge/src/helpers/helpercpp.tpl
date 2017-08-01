
#include "%2.h"


int %2::hash()
{
    return _H("%0");
}
void %2::rawToRosMsg(uint8_t* data)
{
    %3::%4* ptr = new %3::%4;
    //conversion code here
    _rawsize = 0;
    _msg = (void*) ptr; 
}
int %2::rosMsgToRaw(uint8_t** data)
{
    
    %3::%4* ptr = (%3::%4*)_msg;
    int len=0;
    //conversion code here
    return len;
}

