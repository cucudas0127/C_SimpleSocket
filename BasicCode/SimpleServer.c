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
char message[] = "Mr.Server said you rock!";
    
    
struct sockaddr_in st_server, st_client;
//-----------------------------------------------------------------------------
// Function descripts : Read Buffer
//-----------------------------------------------------------------------------
void Svr_ReadBuff(int vSvrFd)
{
    char cJsnBuff[vSvrMax];

    while (read(vSvrFd, cJsnBuff, vSvrMax) > 0)
    {
        printf("\n%s\n", cJsnBuff);
        printf("File received successfully !! \n");

        //sleep(1);        //char message[] = "{\"S3\":\"00\",OPCODE\":\"000\",\"EDGE_ID\":\"E00000000002\",\"EDGE_MSGTIME\":\"20200610214152\"}";
        //write(vSvrFd, message, sizeof(message));
        //printf("Write Success\n");
        //strcpy(cJsnBuff, "abcde");
        //write(vSvrFd, cJsnBuff, vSvrMax);
        //char message[] = "{\"EDGE_ID\":\"E00000000002\",\"EDGE_MSGTIME\":\"20200610214152\",\"OPCODE\":\"000\",\"S1\":\"00\",\"S2\":\"00\",\"S3\":\"21\",\"S4\":\"00\"}";
        //char message[] = "{\"EDGE_ID\":\"E00000000002\",\"EDGE_MSGTIME\":\"20200610214152\",\"OPCODE\":\"000\",\"S3\":\"21\"}";  
    } 

}



//-----------------------------------------------------------------------------
// DESCRIPTS  : Create & Open Socket 
//-----------------------------------------------------------------------------
int OpenSocket_Server(int ip_port)
{
    int socket_fd;
    int itmp;

    // Create TCP/IP Socket & Init
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);           

    if(socket_fd < 0)    return C_FAIL;
    else                 printf("Socket successfully created..\n");

    bzero(&st_server, sizeof(st_server));


    st_server.sin_family = AF_INET;
    st_server.sin_addr.s_addr = htonl(INADDR_ANY);
    st_server.sin_port = htons(ip_port);    


    itmp = bind(vSvrFd, (struct sockaddr *)&st_server, sizeof(st_server));
    if( itmp != 0)  return C_FAIL;
    else            printf("Socket successfully binded..\n");

    return socket_fd;

}


int main()
{

    //---------------------Config--------------------
    int server_port = 8600;

    //================================================
    int sock_fd;
    sock_fd = OpenSocket(server_port);

    if(sock_fd < 0 ) 
    {
        print("Socket Open Error...");
        return;
    }


    while (1)
    {
        if ((listen(vSvrFd, 5)) != 0)
        {
            printf("Listen failed...\n");
            return (fFail);
        }
        else
            printf("Server listening..\n");

        vLen = sizeof(st_client);
        vSvrCntFd = accept(vSvrFd, (struct sockaddr *)&st_client, &vLen);

        if (vSvrCntFd < 0)
        {
            printf("server acccept failed...\n");
            return (fFail);
        }
        else
            printf("server acccept the client...\n");

        Svr_ReadBuff(vSvrCntFd);
        
    }

    close(vSvrFd);

    return 0;
}
