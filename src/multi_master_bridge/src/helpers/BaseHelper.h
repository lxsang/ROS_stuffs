#ifndef BASE_HELPER_H
#define BASE_HELPER_H
#include <ros/ros.h>

#define _H simple_hash
#ifdef __cplusplus
extern "C"
{
#endif
#include "../data_portal.h"

#ifdef __cplusplus
}
#endif
using namespace std;
class BaseHelper{
    public:
        BaseHelper(){};
        ~BaseHelper();
        void* msg();
        int raw(uint8_t**);
        portal_data_t getPortalDataFor(const char*);
        static int hash(){};
        void consume(uint8_t*v,int s){size=s;this->rawToRosMsg(v);};
        void consume(void*v){_msg = v;};
    protected:
        int size;
        virtual void  rawToRosMsg(uint8_t*)=0;
        virtual int rosMsgToRaw(uint8_t**)=0;
        
        void* _msg;
};

#endif