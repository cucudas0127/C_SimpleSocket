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
#define C_SUCCESS 1


//-----------------------------------------------------------------------------
// DESCRIPTS  :Setting IP Address to Socket
//-----------------------------------------------------------------------------
int SCK_SetIPAddress(char *sAddr)
{
  // Check IP format   
  if(strlen(sAddr) < 7)     return C_FAIL;   // C_FAIL      // 1.1.1.1
  //if(!isdigit(sAddr))    return -1;   // 1st Char is not Letter

  strcpy(stSocket.Config.sIPAddr, sAddr);
  //printf("IPAddr = %s\n",stSocket.Config.sIPAddr);
  return C_SUCCESS; // C_SUCCESS
}

//-----------------------------------------------------------------------------
// DESCRIPTS  :Setting PORT to Socket
//-----------------------------------------------------------------------------
int SCK_SetPort(unsigned int iPort)
{
  // Check ...
  if(iPort<0||65535<iPort)          return C_FAIL;   // C_FAIL     if port isnt num

  stSocket.Config.iPort = iPort;
  return C_SUCCESS; // C_SUCCESS
}




//-----------------------------------------------------------------------------
// DESCRIPTS  : Create & Open Socket 
//-----------------------------------------------------------------------------
void SCK_OpenSocket(char *ip_addr, unsigned int ip_port)
{
    int socket_fd;
    int vOpt = 1;       // OptVal = TRUE
    //VCL_PrintfXY(1,1,"%3d",110);

    // Create TCP/IP Socket & Init
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);           
    //VCL_PrintfXY(1,1,"%3d",111);

    if(socket_fd < 0)  { SCK.eHandler.iErr = SCK_C_ERR_CONNECT; return;  }
    //else                    { printf("[SCK] Socket successfully created..\n");  }
    //VCL_PrintfXY(1,1,"%3d",112);

    bzero(&stSocket.HostAddr, sizeof(struct sockaddr_in));
    //VCL_PrintfXY(1,1,"%3d",113);

    stSocket.HostAddr.sin_family      = AF_INET;
    stSocket.HostAddr.sin_addr.s_addr = inet_addr(stSocket.Config.sIPAddr);
    stSocket.HostAddr.sin_port        = htons(stSocket.Config.iPort);
    //printf("stSocket Sockfd   %d\n",socket_fd);
    //VCL_PrintfXY(1,1,"%3d",114);

    if(setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &vOpt, sizeof(vOpt)) == -1)  
    { SCK.eHandler.iErr = SCK_C_ERR_CONNECT; return;  } 
    //VCL_PrintfXY(1,1,"%3d",115);
    
    SCK.fConnectInfo = connect(socket_fd, (struct sockaddr  *)&stSocket.HostAddr, sizeof(stSocket.HostAddr));
    //printf("ConnectInfo   %d\n",SCK.fConnectInfo );

    if(SCK.fConnectInfo != SCK_C_CONNECT_SUCCESS)                  
    { SCK.eHandler.iErr = SCK_C_ERR_CONNECT; return;} 
    // Open Success
    //VCL_PrintfXY(1,1,"%3d",116);

    SCK.eHandler.iErr =SCK_C_ERR_NONE;     //   { printf("connected to the server..\n"); }

}

//-----------------------------------------------------------------------------
// DESCRIPTS  :Initialize Socket [IP Address, Port] and Open Socket
//-----------------------------------------------------------------------------
void SCK_Init(char *ip_addr, unsigned int ip_port)
{
  memset(&stSocket, 0,sizeof(stSCK_Socket));

  // Set Dest. IP Address & Port Number
  if(SCK_SetIPAddress(ip_addr) < 0 )     {SCK.eHandler.iErr=SCK_C_ERR_Init; }     
  if(SCK_SetPort(ip_port) < 0 )           {SCK.eHandler.iErr=SCK_C_ERR_Init; }  
  // Init System (Socket) 

  SCK_OpenSocket();               // Open / Reopen Socket
  //SCK.eHandler.iErr =SCK_C_ERR_NONE;

}


//-----------------------------------------------------------------------------
// DESCRIPTS  :Send data to server 
//-----------------------------------------------------------------------------
void SCK_SendData(void)
{
  int itmp; 

  // Send Packet
  itmp = send(stSocket.Sockfd, stSocket.TxBuf, stSocket.nTxDataSize, MSG_NOSIGNAL);
  stSocket.fTxActivate = 0;  //DeActivate


  // Check Tx Status
  if(itmp < 0)  { SCK.eHandler.iErr=SCK_C_ERR_SEND;          }
  else          { TaskHandler_Make_Under_999(&SCK.nTRX.nTx); }
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
  
    SCK_OpenSocket(server_addr,server_port);
  
  
    // Config Send Packet
    int   send_packet_len = 3;



    // if you want changing rx_buf size 
    // you can config {MAX_RX_BUF_SIZE} in header.h
    // Default rxbuf size : 128
    //================================================

/*
    // Socket Open
    SCK_OpenSocket();

    // Data Send
    SCK_SendData();            

    // Recv Packet
    SCK_RecvPacket();

    // Socket Close
    SCK_Finalize();


*/
}
