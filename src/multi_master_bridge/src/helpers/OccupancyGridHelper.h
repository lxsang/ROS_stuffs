#ifndef OCCUPANCYGRID_HELPER
#define OCCUPANCYGRID_HELPER

#include "BaseHelper.h"
#include "nav_msgs/OccupancyGrid.h" 
class OccupancyGridHelper: public BaseHelper
{
    public: 
        
        OccupancyGridHelper(){ }
        
        // reimplement the hash method
        static int hash();

    protected: 
        // two convertion methods should be reimplemented
        void rawToRosMsg(uint8_t*);
        int rosMsgToRaw(uint8_t**);
};

#endif