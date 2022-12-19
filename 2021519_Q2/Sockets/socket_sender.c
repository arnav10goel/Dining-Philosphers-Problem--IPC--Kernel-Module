#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<sys/un.h>
#include<string.h>

#define MAX_IDS 50
#define STR_LEN 6
#define socket_path "temp_socket"

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

    unsigned int sockfd_sender;
    struct sockaddr_un string_sender;
    int len, server_socket;

    sockfd_sender = socket(AF_UNIX, SOCK_STREAM, 0);
    if(sockfd_sender < 0)
    {
        perror("socket");
        exit(1);
    }

    string_sender.sun_family = AF_UNIX;

    printf("Socket Path: %s\n", socket_path);
    strcpy(string_sender.sun_path, socket_path);
    printf("Socket Path: %s\n", string_sender.sun_path);

    len = strlen(string_sender.sun_path)+1 + sizeof(string_sender.sun_family);
    unlink(socket_path);
    if(bind(sockfd_sender, (struct sockaddr*)&string_sender, len) == -1)
    {
        perror("Socket Connection Error");
        exit(1);
    }

    if(listen(sockfd_sender, 20) == -1)
    {
        perror("Listening Error");
        exit(1);
    }

    int id_sent = 0;
    int id_received = 0;
    while(1){
        // printf("HII");
        server_socket = accept(sockfd_sender, NULL, NULL);
        if(server_socket == -1)
        {
            perror("Accepting Error");
            exit(1);
        }
        for(int j = 0; j < 5; j++){
            if(write(sockfd_sender, listofstrings[id_sent], strlen(listofstrings[id_sent])+1) == -1){
                perror("write");
                exit(0);
            }
            id_sent += 1;
            if(id_received == 5+id_sent){
                id_received = id_sent;
                printf("Highest ID received: %d\n", id_received);
            }
            if(id_sent == 50){
                break;
            }
        }
        close(server_socket);
    }

    close(sockfd_sender);
    exit(0);
}