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

    unsigned int sockfd_sender;
    struct sockaddr_un string_sender;
    int len;

    sockfd_sender = socket(AF_UNIX, SOCK_STREAM, 0);
    if(sockfd_sender < 0)
    {
        perror("socket");
        exit(1);
    }

    string_sender.sun_family = AF_UNIX;
    char* socket_path = "mysocket.socket";
    strcpy(string_sender.sun_path, socket_path);

    len = strlen(string_sender.sun_path) + sizeof(string_sender.sun_family);
    if(connect(sockfd_sender, (struct sockaddr *)&string_sender, len) == -1)
    {
        perror("Socket Connection Error");
        exit(1);
    }

    int id_sent = 0;
    int id_received = 0;
    while(1){
        printf("HII");
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
    }

    close(sockfd_sender);
    exit(0);
}