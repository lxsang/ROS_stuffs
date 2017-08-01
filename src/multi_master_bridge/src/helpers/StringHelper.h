#ifndef STRING_HELPER
#define STRING_HELPER

#include "BaseHelper.h"
#include "std_msgs/String.h" 
class StringHelper: public BaseHelper
{
    public: 
        // inherite constructor
        StringHelper(){ }
        
        // reimplement the hash method
        static int hash();

    protected: 
        // two convertion methods should be reimplemented
        void rawToRosMsg(uint8_t*);
        int rosMsgToRaw(uint8_t**);
};

#endif