#include <string.h>   
#include <unistd.h>  
#include <netdb.h>  
#include <net/if.h>  
#include <arpa/inet.h>  
#include <sys/ioctl.h>  
#include <sys/types.h>  
#include <sys/socket.h>
#include "../include/NetworkTool.h"
#include "../include/Logger.h" 

int NetworkTool::GetLocalIp(const char *eth_inf, char *ip) {
    int sd;  
    struct sockaddr_in sin;  
    struct ifreq ifr;  
  
    if (0 > (sd = socket(AF_INET, SOCK_DGRAM, 0))) {
        LogError() << "NetworkTool:socket";
    }  
  
    strncpy(ifr.ifr_name, eth_inf, IFNAMSIZ);  
    ifr.ifr_name[IFNAMSIZ - 1] = 0;  

    if (0 > ioctl(sd, SIOCGIFADDR, &ifr)) {
        LogError() << "NetworkTool:ioctl";
    }
  
    memcpy(&sin, &ifr.ifr_addr, sizeof(sin));  
    memcpy(ip, inet_ntoa(sin.sin_addr), 16);  
  
    close(sd);  
    return 0;  
}