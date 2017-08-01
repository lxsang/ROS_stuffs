
#include "watchdog.h"

struct inet_id_ read_inet_id(const char* inf)
{
    int fd;
    struct ifreq ifr;
    struct inet_id_ id;
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    //Type of address to retrieve - IPv4 IP address
    ifr.ifr_addr.sa_family = AF_INET;
 
    //Copy the interface name in the ifreq structure
    strncpy(ifr.ifr_name , inf , IFNAMSIZ-1);
     
    //get the ip address
    if(ioctl(fd, SIOCGIFADDR, &ifr) == 0)
        id.ip = ((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr;
     
    //get the netmask ip
    if(ioctl(fd, SIOCGIFNETMASK, &ifr) == 0)
        id.netmask = ((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr;
    //calculate bc address
    id.broadcast.s_addr = id.ip.s_addr | ~ id.netmask.s_addr;
    
    if (ioctl(fd, SIOCGIFHWADDR, &ifr) == 0) 
        memcpy(id.mac, ifr.ifr_hwaddr.sa_data, 6);

    /*MLOG("%s \n",inet_ntoa(id.ip));
    MLOG("%s \n",inet_ntoa(id.netmask));
   
    MLOG("%s \n",inet_ntoa(id.broadcast));

    int i=0;
    for(i;i<6;i++)
        MLOG("%.2X ", id.mac[i]);
    MLOG("\n");*/
    
    close(fd);
    return id;
}

void notify(int interval,int port, const char* iface)
{
    // this should be in a thread
    while(1)
    {
        MLOG("Sending beacon...\n");
        send_beacon(port,iface);
        nanosleep((const struct timespec[]){{0, (long)interval*1000000L}}, NULL);
    }
}

int send_beacon(int port, const char* iface)
{
    int sockfd; 
    struct sockaddr_in their_addr; // connector's address information
    int numbytes;
    int broadcast = 1;
    int magic = MAGIC_HEADER;
    struct inet_id_ id = read_inet_id(iface);
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        return 0;
    }

    // this call is what allows broadcast packets to be sent:
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast,
        sizeof broadcast) == -1) {
        close(sockfd);
        perror("setsockopt (SO_BROADCAST)");
        return 0;
    }

    their_addr.sin_family = AF_INET;     // host byte order
    their_addr.sin_port = htons(port); // short, network byte order
    their_addr.sin_addr = id.broadcast;
    memset(their_addr.sin_zero, '\0', sizeof their_addr.sin_zero);

    if ((numbytes=sendto(sockfd, &magic, 4, 0,
             (struct sockaddr *)&their_addr, sizeof their_addr)) == -1) {
        perror("sendto");
        close(sockfd);
        return 0;
    }
    close(sockfd);
    MLOG("sent %d bytes to %s\n", numbytes,
    inet_ntoa(their_addr.sin_addr));
    if(numbytes != 4) return 0;
    return 1;
}

struct in_addr* sniff_beacon(int port)
{
    int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int numbytes;
	struct sockaddr_storage their_addr;
	char buf[MAX_BUFF];
	socklen_t addr_len;
	char s[INET6_ADDRSTRLEN];
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE; // use my IP
    char port_a[10];
    snprintf(port_a, 10,"%d",port);
	if ((rv = getaddrinfo(NULL, port_a, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return NULL;
	}
	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("listener: socket");
			continue;
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("listener: bind");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "listener: failed to bind socket\n");
        close(sockfd);
		return 0;
	}
	freeaddrinfo(servinfo);

    struct timeval read_timeout;
    read_timeout.tv_sec = 0;
    read_timeout.tv_usec = 300000;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof read_timeout);

	MLOG("listener: waiting to recvfrom...\n");

	addr_len = sizeof their_addr;
	if ((numbytes = recvfrom(sockfd, buf, MAX_BUFF-1 , 0,
		(struct sockaddr *)&their_addr, &addr_len)) == -1) {
		//perror("recvfrom");
        close(sockfd);
		return NULL;
	}
    close(sockfd);
    int* v = (int*)buf;
    if(numbytes == 4 && *v == MAGIC_HEADER)
    {
        struct sockaddr * sa = (struct sockaddr *)&their_addr;
        if (sa->sa_family == AF_INET) {
		    return &(((struct sockaddr_in*)sa)->sin_addr);
	    }
        //IPV6
	    //return &(((struct sockaddr_in6*)sa)->sin6_addr);
        return NULL;
    }
    else
        return NULL;
}