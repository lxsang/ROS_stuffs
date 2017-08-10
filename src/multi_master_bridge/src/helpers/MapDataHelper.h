#ifndef MAPDATA_HELPER
#define MAPDATA_HELPER

#include "BaseHelper.h"
#include "multi_master_bridge/MapData.h" 
class MapDataHelper: public BaseHelper
{
    public: 
        
        MapDataHelper(){ }
        
        // reimplement the hash method
        static int hash();

    protected: 
        // two convertion methods should be reimplemented
        void rawToRosMsg(uint8_t*);
        int rosMsgToRaw(uint8_t**);
};

#endif