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
    char listofstrings[MAX_IDS][STR_LEN+1];
    char listofalphabets[26][2] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k",
                                "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v",
                                "w", "x", "y", "z"};
    
    for(int i = 0; i < MAX_IDS; i++){
        for(int j = 0; j < STR_LEN; j++){
            listofstrings[i][j] = listofalphabets[rand()%26][0];
        }
        listofstrings[i][STR_LEN] = '\0';    
    }

    unsigned int sockfd;
    struct sockaddr_un string_sender;
    char buffer[1024];
    int len;

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(sockfd == -1)
    {
        perror("Socket Creation Failed");
        exit(0);
    }
    string_sender.sun_family = AF_UNIX;
    char* socket_path = "sender_socket";
    strcpy(string_sender.sun_path, socket_path);
    unlink(string_sender.sun_path); //Unlink to avoid errors

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