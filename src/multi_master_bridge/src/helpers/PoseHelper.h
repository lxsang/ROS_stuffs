#ifndef POSE_HELPER
#define POSE_HELPER

#include "BaseHelper.h"
#include "geometry_msgs/Pose.h" 
class PoseHelper: public BaseHelper
{
    public: 
        
        PoseHelper(){ }
        
        // reimplement the hash method
        static int hash();

    protected: 
        // two convertion methods should be reimplemented
        void rawToRosMsg(uint8_t*);
        int rosMsgToRaw(uint8_t**);
};

#endif