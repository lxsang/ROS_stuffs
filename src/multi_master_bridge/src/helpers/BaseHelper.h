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
        void consume(uint8_t*v,int s){_rawsize=s;this->rawToRosMsg(v);};
        void consume(void*v){_msg = v;};
        int rawsize(){return _rawsize;};
        template <class FT> void publish(ros::Publisher*);
    protected:
        int _rawsize;
        virtual void  rawToRosMsg(uint8_t*)=0;
        virtual int rosMsgToRaw(uint8_t**)=0;
        
        void* _msg;
};

template <class FT> void BaseHelper::publish(ros::Publisher* pub)
{
    if(_msg && pub)
        pub->publish(*((FT*)_msg));
}

#endif