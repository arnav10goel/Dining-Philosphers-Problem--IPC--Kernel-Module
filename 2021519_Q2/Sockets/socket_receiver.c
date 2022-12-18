#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<sys/un.h>
#include<string.h>

int main()
{
    int sockfd;
    struct sockaddr_un server_addr;
    char buffer[1024];
    int len;

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        perror("socket");
        exit(1);
    }

    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, "server_socket");
    if(connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("connect");
        exit(1);
    }

    printf("Enter the string to send: ");
    scanf("%s", buffer);
    len = send(sockfd, buffer, strlen(buffer), 0);
    if(len < 0)
    {
        perror("send");
        exit(1);
    }

    close(sockfd);
    return 0;
}