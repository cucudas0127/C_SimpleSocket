
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main (void)

{
  
    int sockfd, send_len;
   	struct sockaddr_in multicast_addr;	
    struct in_addr     local_addr;

    // Max Router Num
    char ttl=5;
  
    // multicast addr
    char *multicast_ip = "225.192.0.10";
    int   multicast_port  = 50000;

    // send device ip address [current computer]
    char *local_ip      = "10.1.16.104";

    // sending message
    char *send_message ="Hello multicast";

    // generatae socket
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0) 
    {
        printf("Socket open error\n");
        return -1;
    }

    // setting multicast address
    multicast_addr.sin_family      = AF_INET;
    multicast_addr.sin_addr.s_addr = inet_addr(multicast_ip);
    multicast_addr.sin_port        = htons(multicast_port);

    // set the maximum number of routers.
    if(setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_TTL,(void *)&ttl,sizeof(ttl))<0)
    {
        printf("Socket setsickopt error.[ttl]\n");

        close(sockfd);
        return -1;
    }

    // set the Ip address for ethernet port(local ip)
    local_addr.s_addr = inet_addr(local_ip);    
    if(setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_IF,(char *)&local_addr, sizeof(local_addr))<0)
    {
        printf("Socket setsickopt error.[Interface]\n");
        close(sockfd);
        return -1;
    }
    // Multicast Option
    // IPPROTO_IP  : 
    // To manipulate options at the IPv4 or IPv6 level, the level 
    // parameter must be set to IPPROTO_IP as defined in sys/socket.h 
    // or IPPROTO_IPV6 as defined in netinet/in.h.

    // IP_MULTICAST_TTL :
    // (RAW and UDP) Sets the IP time-to-live of outgoing multicast datagrams. 
    // Default value is 1 (that is, multicast only to the local subnet). 
    // The TTL value is passed as an u_char.

    // IP_MULTICAST_IF :
    // Usually, the system administrator specifies the default interface 
    // multicast datagrams should be sent from. The programmer can override 
    // this and choose a concrete outgoing interface for a given socket 
    // with this option.    
   
    // send message
    send_len = sendto(sockfd, send_message, strlen(send_message), 0, (struct sockaddr*)&multicast_addr, sizeof(multicast_addr));
    printf("send len : %d\n",send_len);
    if( send_len < 0 )
    {
        printf("Sending multicast message error");
    }
    else
        printf("Sending multicast message...Success\n");


    close(sockfd);
    return 0;
}
