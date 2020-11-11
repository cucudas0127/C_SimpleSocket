//-----------------------------------------------------------------------------
// PROGRAMMER   : cucudas0127
// REVISION     : 2020.11.05
// DESCRIPTS    : Simple Socket Client Code 
// Environment ----------------------------------------------------------------
// OS : ubuntu 18.04
//-----------------------------------------------------------------------------
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>         // O_WRONLY
#include <unistd.h>        // write(), close()


#define C_FAIL    -1
#define C_SUCCESS 0

#define MAX_RX_BUR_SIZE  128

// Server's Host Address 
struct sockaddr_in	    HostAddr;                   

//-----------------------------------------------------------------------------
// DESCRIPTS  : Create & Open Socket 
//-----------------------------------------------------------------------------
int OpenSocket_Client(char *ip_addr,int ip_port)
{
    int socket_fd;
    int itmp;
    int vOpt = 1;       // OptVal = TRUE

    // Create TCP/IP Socket & Init
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);           

    if(socket_fd < 0)    return C_FAIL;
    else                 printf("Socket successfully created..\n");

    bzero(&HostAddr, sizeof(struct sockaddr_in));

    HostAddr.sin_family      = AF_INET;
    HostAddr.sin_addr.s_addr = inet_addr(ip_addr);
    HostAddr.sin_port        = htons(ip_port);

    itmp = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &vOpt, sizeof(vOpt));

    if(itmp != 0)  return C_FAIL;  
    
    itmp =connect(socket_fd, (struct sockaddr  *)&HostAddr, sizeof(HostAddr));
    if(itmp != 0 ) return C_FAIL;


    // Open Success
    printf("Socket successfully connected..\n");
    return socket_fd;

}



//-----------------------------------------------------------------------------
// DESCRIPTS  :Send data to server 
//-----------------------------------------------------------------------------
int SendData(int sock_fd, char *tx_buf,int tx_len)
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
int DataRecived(int sock_fd, char *rx_buf, int rx_buf_size)
{
  int  	    nData;
  int	    	hostAddr_size = sizeof(struct sockaddr_in);
  struct    timeval  timeOut;
  fd_set  	readFds ;
  
  memset(rx_buf, 0, rx_buf_size);

  // Set 1ms Timeout counter
  // Just wait 1 msecond and exit
  timeOut.tv_sec  = 0;
  timeOut.tv_usec = 1000;   

  FD_ZERO(&readFds);
  FD_SET(sock_fd, &readFds);



  // if Recivedata Exist
  if(select(sock_fd+1, &readFds, NULL, NULL, &timeOut) > 0) 
  {
    nData = recvfrom(sock_fd, rx_buf, rx_buf_size, 0,
                     (struct sockaddr *)&HostAddr, &hostAddr_size);
    return C_SUCCESS;
  }  
  else return C_FAIL;

}


//-----------------------------------------------------------------------------
// DESCRIPTS  :Close the Socket
//-----------------------------------------------------------------------------
void CloseSocket(int sock_fd)
{
  if(sock_fd > 0 )              
    close(sock_fd);
}



//-----------------------------------------------------------------------------
// DESCRIPTS  :Management Socket
//-----------------------------------------------------------------------------
void  main(void)
{
    //---------------------Config--------------------
    // Enter Server IP 
    char* server_addr = "127.0.0.1";
    int   server_port = 8600;
  
    char* send_data="hello world!!";
  
    //================================================
    int   sock_fd, itmp;
    int   send_data_len;
    char  rx_buf[MAX_RX_BUR_SIZE] = {0,};
    send_data_len = strlen(send_data);

    // Socket Open
    sock_fd = OpenSocket_Client(server_addr,server_port);
    if(sock_fd == C_FAIL) return;

    // Data Send
    itmp = SendData(sock_fd, send_data, send_data_len);
    if(itmp != C_FAIL) printf("Send Data Success!\n");

    // Recv Packet
    
    //while(!DataRecived(sock_fd, rx_buf, MAX_RX_BUR_SIZE));
    DataRecived(sock_fd, rx_buf, MAX_RX_BUR_SIZE);
    printf("Recived Data : %s",rx_buf);
    // Socket Close
    CloseSocket(sock_fd);

}
