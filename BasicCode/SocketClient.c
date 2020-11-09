//-----------------------------------------------------------------------------
// PROGRAMMER   : cucudas0127
// REVISION     : 2020.11.05
// DESCRIPTS    : Simple Socket Communication 
// Environment ----------------------------------------------------------------
// OS : ubuntu 18.04
//-----------------------------------------------------------------------------
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define C_FAIL    -1
#define C_SUCCESS 0


//-----------------------------------------------------------------------------
// DESCRIPTS  : Create & Open Socket 
//-----------------------------------------------------------------------------
int OpenSocket(char *ip_addr, unsigned int ip_port)
{
    int socket_fd;
    int itmp;
    int vOpt = 1;       // OptVal = TRUE

    // Create TCP/IP Socket & Init
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);           

    if(socket_fd < 0)    return C_FAIL;
    else                 printf("Socket successfully created..\n");

    bzero(&stSocket.HostAddr, sizeof(struct sockaddr_in));

    stSocket.HostAddr.sin_family      = AF_INET;
    stSocket.HostAddr.sin_addr.s_addr = inet_addr(stSocket.Config.sIPAddr);
    stSocket.HostAddr.sin_port        = htons(stSocket.Config.iPort);

    itmp = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &vOpt, sizeof(vOpt);

    if(itmp != 0)  return C_FAIL;  
    
    itmp =connect(socket_fd, (struct sockaddr  *)&stSocket.HostAddr, sizeof(stSocket.HostAddr);
    if(itmp != 0 ) return C_FAIL;


    // Open Success
    printf("Socket successfully connected..\n");
    return C_SUCCESS;

}



//-----------------------------------------------------------------------------
// DESCRIPTS  :Send data to server 
//-----------------------------------------------------------------------------
int SendData_toServer(int sock_fd, char *tx_buf,int tx_len)
{
  int itmp; 

  // Send Packet
  itmp = send(sock_fd, tx_buf, tx_len, MSG_NOSIGNAL);

  // Check Tx Status
  if(itmp < 0)  return C_FAIL;   
  else          return C_SUCCESS; 
}





//-----------------------------------------------------------------------------
// DESCRIPTS  :Recive data from server 
//-----------------------------------------------------------------------------
void SCK_RecvPacket(int sock_fd, char *rx_buf, int rx_buf_size)
{
  int  	    nData;
  int		hostAddr_size = sizeof(struct sockaddr_in);
  struct    timeval  timeOut;
  fd_set  	readFds ;
  
  memset(rx_buf, 0, rx_buf_size);

  // Set 1ms Timeout counter
  timeOut.tv_sec  = 0;
  timeOut.tv_usec = 1000;   

  FD_ZERO(&readFds);
  FD_SET(sock_fd, &readFds);

  if(select(sock_fd+1, &readFds, NULL, NULL, &timeOut) > 0) 
  {
    nData = recvfrom(sock_fd, rx_buf[0], rx_buf_size, 0,
                     (struct sockaddr *)&stSocket.HostAddr, &hostAddr_size);

  }  

}



//-----------------------------------------------------------------------------
// DESCRIPTS  :Management Socket
//-----------------------------------------------------------------------------
void  main(void)
{
    //---------------------Config--------------------
    // Config Serial Port
    char* server_addr = "127.0.0.1";
    int   server_port = 8600;
  
  
  
    // Config Send Packet
    int   send_packet_len = 3;



    // if you want changing rx_buf size 
    // you can config {MAX_RX_BUF_SIZE} in header.h
    // Default rxbuf size : 128
    //================================================

    // Socket Open
    OpenSocket(server_addr,server_port);

/*
    SCK_OpenSocket();

    // Data Send
    SCK_SendData();            

    // Recv Packet
    SCK_RecvPacket();

    // Socket Close
    SCK_Finalize();


*/
}
