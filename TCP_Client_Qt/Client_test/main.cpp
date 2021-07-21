#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <winsock.h>

using namespace std;

WORD wVersionRequested;
WSADATA wsaData;

int main(int argc, char* argv[])
{
    wVersionRequested = MAKEWORD(2, 2);
    WSAStartup(wVersionRequested, &wsaData);
    perror("WSAStartup error");

    if(argc != 3)
    {
        cout << "Using:./client ip port\nExample:./client 127.0.0.1 5005" << endl;
    }

    //step 1. Creat socket for client
    int sockfd;
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("sockfd = %d\n", sockfd);
        perror("socket");
    }

    //step 2. Connect to server
    struct hostent *h;
    if((h = gethostbyname(argv[1])) == 0) //get ip of server
    {
        cout << "gethostname failed.\n" << endl;
        close(sockfd);
        return -1;
    }
    for(int i=0; h->h_addr_list[i]; i++){
            printf("IP addr %d: %s\n", i+1, inet_ntoa( *(struct in_addr*)h->h_addr_list[i] ) );
        }
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[2]));
    //servaddr.sin_addr.S_un.S_addr = inet_addr(argv[1]);
    memcpy(&servaddr.sin_addr, h->h_addr, h->h_length);
    printf("IP = %s , Port = %s\n", argv[1], argv[2]);
    if(connect(sockfd, (struct sockaddr *)&servaddr,sizeof(servaddr)) != 0)
    {
        perror("connect");
        cerr << GetLastError() << endl;
    }
    char *buffer = new char[1024];

/*
    //step 2. Connect to server
    SOCKADDR_IN addrto;            //发往的地址
    memset(&addrto, 0, sizeof(addrto));
    addrto.sin_family = AF_INET;
    addrto.sin_addr.S_un.S_addr = inet_addr("130.61.250.117");//130.61.250.117 17480 216.58.204.4
    //端口号必须和服务器绑定的端口号一致
    addrto.sin_port = htons(22);
    //申请链接服务器
    if (connect(sockfd, (SOCKADDR*)&addrto, sizeof(SOCKADDR))==0)printf("connect success!\n");
    else printf("connect failed!\n");
    cout << GetLastError() << endl;
      char buffer[1024];
*/
    //step 3. Communicate with server
    for(int i = 0; i < 5; i++)
    {
        int iret;
        memset(buffer, 0, 1024);
        strcpy(buffer ,"I am client!\n");
        if((iret = send(sockfd, buffer, strlen(buffer), 0)) <= 0)
        {
            printf("Failed to build up a connection!\n");
            perror("Send");
            close(sockfd);
            break;
        }
        cout << "Send: " << buffer << endl;
        memset(buffer, 0, 1024);
        if((iret = recv(sockfd, buffer, 1024, 0)) <= 0)
        {
            perror("Recv");
            cout << "error number is "<< iret << endl;
            close(sockfd);
            break;
        }
        cout << "Recv: " << buffer << endl;
    }

    //step 4. Close connection
    close(sockfd);
    //delete [] buffer;
    return 0;
}
