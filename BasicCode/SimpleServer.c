//-----------------------------------------------------------------------------
// PROGRAMMER   : cucudas0127
// REVISION     : 2020.11.10
// DESCRIPTS    : Simple Socket Server Code  
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
#include <netinet/in.h>

#define vSvrMax 1024


#define C_SUCCESS 0
#define C_FAIL 1

int flag = 0;
    
    struct sockaddr_in st_server;
   
//-----------------------------------------------------------------------------
// DESCRIPTS  :Send data to server 
//-----------------------------------------------------------------------------
int SendData(int sock_fd, char *tx_buf,int tx_len)
{
  int itmp; 
  //printf("Send Data : %s\n",tx_buf);
  
  // Send Packet
  itmp = send(sock_fd, tx_buf, tx_len);

  // Check Tx Status
  if(itmp < 0)  return C_FAIL;   
  else          return C_SUCCESS; 
}


//-----------------------------------------------------------------------------
// Function descripts : Read Buffer
//-----------------------------------------------------------------------------
void Read_RecivedData(int vSvrFd)
{
    char cJsnBuff[vSvrMax];

    read(vSvrFd, cJsnBuff, vSvrMax);
    printf("\n%s\n", cJsnBuff);
    printf("File received successfully !! \n");
}

//-----------------------------------------------------------------------------
// DESCRIPTS  :Recive data from server 
//-----------------------------------------------------------------------------
int RecivedData(int sock_fd, char *rx_buf, int rx_buf_size)
{
  int  	    nData;
  int	    hostAddr_size = sizeof(struct sockaddr_in);
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
                     (struct sockaddr *)&st_server, &hostAddr_size);
    return C_SUCCESS;
  }  
  else return C_FAIL;

}

//-----------------------------------------------------------------------------
// DESCRIPTS  : Create & Open Socket 
//-----------------------------------------------------------------------------
int OpenSocket_Server(struct sockaddr_in st_server, int ip_port)
{
    int socket_fd;
    int itmp;

    // Create TCP/IP Socket & Init
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);           
    if(socket_fd < 0)    return C_FAIL;

    // Init socket struct
    bzero(&st_server, sizeof(st_server));

    st_server.sin_family = AF_INET;
    st_server.sin_addr.s_addr = htonl(INADDR_ANY);
    st_server.sin_port = htons(ip_port);    

    // Registering a socket in the kernel
    itmp = bind(socket_fd, (struct sockaddr *)&st_server, sizeof(st_server));
    if( itmp != 0)  return C_FAIL;

    return socket_fd;
}

//-----------------------------------------------------------------------------
// DESCRIPTS  : Client Accept
//-----------------------------------------------------------------------------
int AcceptClient(int sock_fd)
{
    struct sockaddr_in st_client;
    int itmp = sizeof(st_client);
    int client_fd;

    //Check client connection request
    if ((listen(sock_fd, 5)) != 0) 
        return C_FAIL;
    
    
    //accept client 
    client_fd = accept(sock_fd, (struct sockaddr *)&st_client, &itmp);
    if ( client_fd < 0) 
        return C_FAIL;
    
    //if everythin is good
    return client_fd;
}



#define MAX_RX_BUF_SIZE 128

void main(void)
{

    //---------------------Config--------------------
    int server_port = 8600;
    char* send_message= "We are connected!!";
    //================================================
    char rx_buf[128]={0,};
    int sock_fd;
    int itmp;
    int client_fd;

    // Open Socket
    sock_fd = OpenSocket_Server(st_server, server_port);
    if(sock_fd < 0 ) 
    {
        printf("server socket Open Error...\n");
        return;
    }


    // Client Accept
    client_fd = AcceptClient(sock_fd);
    if( client_fd == C_FAIL)
    {
        printf("server accept Error...\n");
        return;
    }

    for(int i = 5; i> 0; i--)
    {
        // Read Data
        //Read_RecivedData(client_fd);
        while(RecivedData(client_fd, rx_buf, MAX_RX_BUF_SIZE)==C_FAIL);
        printf("[%d] Recived Data : %s\n",i,rx_buf);

        // Send Data
        SendData(client_fd,send_message,strlen(send_message));
    }

    //close socket
    close(sock_fd);

    return ;
}


