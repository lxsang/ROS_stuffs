#ifndef QUATERNION_HELPER
#define QUATERNION_HELPER

#include "BaseHelper.h"
#include "geometry_msgs/Quaternion.h" 
class QuaternionHelper: public BaseHelper
{
    public: 
        
        QuaternionHelper(){ }
        
        // reimplement the hash method
        static int hash();

    protected: 
        // two convertion methods should be reimplemented
        void rawToRosMsg(uint8_t*);
        int rosMsgToRaw(uint8_t**);
};

#endif