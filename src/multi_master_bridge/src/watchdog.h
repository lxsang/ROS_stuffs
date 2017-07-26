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

#ifndef MLOG
#define MLOG printf
#endif
#define MAX_BUFF 512
// four bytes magic header
#define  MAGIC_HEADER 0xAB2F374A
struct inet_id_ {
    struct in_addr ip;
    struct in_addr netmask;
    struct in_addr  broadcast;
    unsigned char mac[6];
};

struct inet_id_ read_inet_id(const char* );
// send hello beacon each <interval> milliseconds
void notify(int,int,const char*);
int send_beacon(int,const char*);
struct in_addr* sniff_beacon(int);
//void watch();
#endif 