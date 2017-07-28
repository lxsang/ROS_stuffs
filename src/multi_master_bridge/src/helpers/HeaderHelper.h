#ifndef HEADER_HELPER
#define HEADER_HELPER

#include "BaseHelper.h"
#include "std_msgs/Header.h" 
class HeaderHelper: public HeaderHelper
{
    public: 
        
        HeaderHelper(){ }
        
        // reimplement the hash method
        static int hash();

    protected: 
        // two convertion methods should be reimplemented
        void rawToRosMsg(uint8_t*);
        int rosMsgToRaw(uint8_t**);
};

#endif