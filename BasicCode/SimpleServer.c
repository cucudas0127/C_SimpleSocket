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
#define vSvrPort 8200
#define fSuccess 0
#define fFail 1

int flag = 0;
char message[] = "Mr.Server said you rock!";

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

int main()
{
    int vSvrFd, vSvrCntFd, vLen;
    struct sockaddr_in stSvrAddr, stClntAddr;

    vSvrFd = socket(AF_INET, SOCK_STREAM, 0);

    if (vSvrFd == -1)
    {
        printf("socket creation failed...\n");
        return (fFail);
    }
    else
        printf("Socket successfully created..\n");

    bzero(&stSvrAddr, sizeof(stSvrAddr));

    stSvrAddr.sin_family = AF_INET;
    stSvrAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    stSvrAddr.sin_port = htons(vSvrPort);    
        if ((bind(vSvrFd, (struct sockaddr *)&stSvrAddr, sizeof(stSvrAddr))) != 0)
        {
            printf("socket bind failed...\n");
            return (fFail);
        }
        else
            printf("Socket successfully binded..\n");

        while (1)
        {
            if ((listen(vSvrFd, 5)) != 0)
            {
                printf("Listen failed...\n");
                return (fFail);
            }
            else
                printf("Server listening..\n");

            vLen = sizeof(stClntAddr);
            vSvrCntFd = accept(vSvrFd, (struct sockaddr *)&stClntAddr, &vLen);

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
