#ifndef UTIL_H
#define UTIL_H
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netdb.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

#include "data_portal.h"

#ifndef MLOG
#define MLOG printf
#endif
#define MAX_BUFF 512
#define TIME_OUT_S 3
#define TIME_OUT_U 300000
// four bytes magic header
#define  MAGIC_HEADER 0xAB2F374A
#define  PRAGMENT_SIZE 65500
//#define  MAX_DGRAM_DATA 65500
struct inet_id_ {
    struct in_addr ip;
    struct in_addr netmask;
    struct in_addr  broadcast;
    char* hostname;
    unsigned char mac[6];
};

struct beacon_t {
    struct in_addr ip;
    char* hostname;
    int port;
    int status;
};
struct inet_id_ read_inet_id(const char* );
//void notify(int,int,const char*);
int send_beacon(int,const char*,int);
int bind_udp_socket(int);
struct beacon_t sniff_beacon(int, struct inet_id_);
struct portal_data_t udp_portal_checkin(int, struct inet_id_ id, int* alive);
int upd_data_broadcast(int port, const char* iface, struct portal_data_t pdata);
#endif 