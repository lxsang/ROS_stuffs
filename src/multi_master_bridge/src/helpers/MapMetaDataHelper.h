#ifndef MAPMETADATA_HELPER
#define MAPMETADATA_HELPER

#include "BaseHelper.h"
#include "nav_msgs/MapMetaData.h" 
class MapMetaDataHelper: public BaseHelper
{
    public: 
        
        MapMetaDataHelper(){ }
        
        // reimplement the hash method
        static int hash();

    protected: 
        // two convertion methods should be reimplemented
        void rawToRosMsg(uint8_t*);
        int rosMsgToRaw(uint8_t**);
};

#endif