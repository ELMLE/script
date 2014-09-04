/*--------------------------------------------------------------
 *  Desc: get LAN ip address
 *  Date: 2014-09-01
 *--------------------------------------------------------------*/

#include <stdio.h>
#include <arpa/inet.h>
#include <ifaddrs.h>

bool GetLocalIp(uint32_t *ip) {

    /*
    struct in_addr addr;
    inet_aton("10.0.0.0", &addr);
    uint32_t ip10 = addr.s_addr;
    inet_aton("255.0.0.0", &addr);      // 10.0.0.0/8
    uint32_t ip10_mask = addr.s_addr;

    inet_aton("172.16.0.0", &addr);
    uint32_t ip172 = addr.s_addr;
    inet_aton("255.240.0.0", &addr);    // 172.16.0.0/12
    uint32_t ip172_mask = addr.s_addr;

    inet_aton("192.168.0.0", &addr);
    uint32_t ip192 = addr.s_addr;
    inet_aton("255.255.0.0", &addr);    // 192.168.0.0/16
    uint32_t ip192_mask = addr.s_addr;
    */

    static const uint32_t ip10 = 10;
    static const uint32_t ip10_mask = 255;
    static const uint32_t ip172 = 4268;
    static const uint32_t ip172_mask = 61695;
    static const uint32_t ip192 = 43200;
    static const uint32_t ip192_mask = 65535;

    *ip = 0;

    struct ifaddrs *ptr = NULL;
    if (getifaddrs(&ptr) != 0) {
        fprintf(stderr, "getifaddrs() error: %m.\n");
        return false;
    }

    for (struct ifaddrs *i = ptr; i != NULL; i = i->ifa_next) {
        if (i->ifa_addr->sa_family == AF_INET) { // ipv4
            uint32_t addr = *reinterpret_cast<uint32_t*>(
                &((struct sockaddr_in*)i->ifa_addr)->sin_addr);
            if ((addr & ip10_mask) == ip10 ||
                (addr & ip172_mask) == ip172 ||
                (addr & ip192_mask) == ip192) {
                *ip = addr;
                break;
            }
        }
    }

    if (ptr != NULL) {
        freeifaddrs(ptr);
    }
    return (*ip != 0);
}

int main()
{
    uint32_t ip;
    struct in_addr addr;

    if ( GetLocalIp( &ip ) ) {
        addr.s_addr = ip;
        printf("%s\n", inet_ntoa( addr ));
    }

    return 0;
}
