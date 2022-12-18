#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<sys/un.h>
#include<string.h>

#define MAX_IDS 50
#define STR_LEN 6

int main()
{
    //Code snippet to generate 50 random strings of length 5
    char listofstrings[MAX_IDS][STR_LEN+2];
    char listofalphabets[26][2] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k",
                                "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v",
                                "w", "x", "y", "z"};
    
    for(int i = 0; i < 50; i++){
        for(int j = 0; j < STR_LEN; j++){
            listofstrings[i][j] = listofalphabets[rand()%26][0];
        }
        listofstrings[i][STR_LEN] = i+1;
        listofstrings[i][STR_LEN+1] = '\0';    
    }

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