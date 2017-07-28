#ifndef DATA_PORTAL_H
#define DATA_PORTAL_H
#include <pthread.h>
#include "watchdog.h"


struct portal_data_t{
    char        from[20];
    unsigned    hash;
    int         size;
    uint8_t*    data;
    int         status;
};

struct portal_callback_t{
    int client;
    void (*callback)(struct portal_data_t);
};

int portal_request(const char*,int);
int teleport_raw_data(const char*,int,struct portal_data_t);
int portal_startup(unsigned * );
int portal_listen(int sock);
void portal_serve(int,void (*callbacl)(struct portal_data_t));
void portal_checkin(void*);
unsigned simple_hash(const char*);
#endif