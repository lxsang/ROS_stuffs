#ifndef TEST_HELPER
#define TEST_HELPER

#include "BaseHelper.h"
#include "std_msgs/Test.h" 
class TestHelper: public TestHelper
{
    public: 
        
        TestHelper(){ }
        
        // reimplement the hash method
        static int hash();

    protected: 
        // two convertion methods should be reimplemented
        void rawToRosMsg(uint8_t*);
        int rosMsgToRaw(uint8_t**);
};

#endif