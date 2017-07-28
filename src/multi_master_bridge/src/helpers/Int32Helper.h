#ifndef INT32_HELPER
#define INT32_HELPER

#include "BaseHelper.h"
#include "std_msgs/Int32.h" 
class Int32Helper: public Int32Helper
{
    public: 
        Int32Helper(){ }
        
        // reimplement the hash method
        static int hash();

    protected: 
        // two convertion methods should be reimplemented
        void rawToRosMsg(uint8_t*);
        int rosMsgToRaw(uint8_t**);
};

#endif