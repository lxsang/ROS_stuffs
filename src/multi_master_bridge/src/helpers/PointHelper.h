#ifndef POINT_HELPER
#define POINT_HELPER

#include "BaseHelper.h"
#include "geometry_msgs/Point.h" 
class PointHelper: public BaseHelper
{
    public: 
        
        PointHelper(){ }
        
        // reimplement the hash method
        static int hash();

    protected: 
        // two convertion methods should be reimplemented
        void rawToRosMsg(uint8_t*);
        int rosMsgToRaw(uint8_t**);
};

#endif