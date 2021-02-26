/* Send Multicast Datagram code example. */

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main (void)

{
    int sockfd;
   	struct sockaddr_in sockaddr;	
    struct in_addr     localInterface;
    int send_len;
    char *send_sentence="Hello multicast";


    // Max Router Num
    char ttl=5;
  
    // multicast addr
    char *multicastAddr = "225.192.0.10";
    int   multicastPort = 50000;
    char *local_Ip      = "192.168.0.10";


    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0) 
    {
        printf("Socket open error\n");
        return -1;
    }

    sockaddr.sin_family      = AF_INET;
    sockaddr.sin_addr.s_addr = inet_addr(multicastAddr);
    sockaddr.sin_port        = htons(multicastPort);

    if(setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_TTL,(void *)&ttl,sizeof(ttl))<0)
    {
        printf("Socket setsickopt error.[ttl]\n");

        close(sockfd);
        return -1;
    }

    localInterface.s_addr = inet_addr(local_Ip);    
    if(setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_IF,(char *)&localInterface, sizeof(localInterface))<0)
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
   


    // groupSock sockaddr structure. 

    // int datalen = 1024;
    send_len = sendto(sockfd, send_sentence, strlen(send_sentence), 0, (struct sockaddr*)&sockaddr, sizeof(sockaddr));
    printf("send len : %d\n",send_len);
    if( send_len < 0 )
    {
        printf("Sending multicast message error");
    }
    else
        printf("Sending multicast message...OK\n");

    // Try the re-read from the socket if the loopback is not disable
    return 0;
}

