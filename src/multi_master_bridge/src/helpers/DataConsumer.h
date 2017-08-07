#ifndef DATA_CONSUMER_H
#define DATA_CONSUMER_H

#include "helper.h"
#include "../bridge/data_portal.h"

#include "std_msgs/Header.h"
#include "std_msgs/String.h"
#include "std_msgs/Int32.h"

#define INPUT this->template __consume
template <class T1, class T2> 
class Consumer {
    public: 
        Consumer(){};
        ~Consumer(){};
        void consume(ros::Publisher**,portal_data_t*);
};

class DataConsumer{
    public: 
        DataConsumer(){}
        void consume(ros::Publisher**,portal_data_t*);
    private: 
        template <class FT1,class F2> 
        void __consume(ros::Publisher**,portal_data_t*);
};


template<class FT1,class FT2> 
void DataConsumer::__consume(ros::Publisher**pub,portal_data_t*pd)
{
    Consumer<FT1,FT2> cs;
    cs.consume(pub,pd);
}

#endif