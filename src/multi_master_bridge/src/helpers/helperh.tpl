#ifndef %1_HELPER
#define %1_HELPER

#include "BaseHelper.h"
#include "%0.h" 
class %2: public BaseHelper
{
    public: 
        
        %2(){ }
        
        // reimplement the hash method
        static int hash();

    protected: 
        // two convertion methods should be reimplemented
        void rawToRosMsg(uint8_t*);
        int rosMsgToRaw(uint8_t**);
};

#endif