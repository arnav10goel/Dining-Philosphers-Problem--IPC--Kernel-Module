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
    unsigned int sockfd_recv;
    struct sockaddr_un string_receiver;
    memset(&string_receiver, 0, sizeof(string_receiver));
    int len, server_socket;

    unlink(socket_path);

    sockfd_recv = socket(AF_UNIX, SOCK_STREAM, 0);
    if(sockfd_recv == -1)
    {
        perror("Socket Creation Failed");
        exit(1);
    }
    string_receiver.sun_family = AF_UNIX;
    
    strcpy(string_receiver.sun_path, socket_path);

    if(bind(sockfd_recv, (const struct sockaddr *)&string_receiver, sizeof(struct sockaddr_un)) == -1)
    {
        perror("Binding Error");
    }
    printf("Socket Path: %s\n", string_receiver.sun_path);
    printf("Bind Call Successfull\n");

    int ret = listen(sockfd_recv, 20);
    if(ret == -1)
    {
        perror("Listening Error");
        exit(1);
    }

    int id_received = 0;
    int highest_id = 0; 
    char string_received[STR_LEN+1];

    while(1){
        printf("Waiting on accept() sys call\n");
        server_socket = accept(sockfd_recv, (struct sockaddr *)&string_receiver, (socklen_t*)&len);
        if(server_socket == -1)
        {
            perror("Accept Error");
            exit(1);
        }
        printf("Accept Call Successfull\n");

        while(1){
            if(read(server_socket, string_received, sizeof(string_received)) == -1)
            {
                perror("Reading Error");
                exit(1);
            }
            id_received++;
            printf("ID Received: %d String Received: %s\n", id_received, string_received);
            memset(string_received, 0, sizeof(string_received));
            if(id_received == highest_id + 5){
                highest_id = id_received;
                if(write(server_socket, &highest_id, sizeof(highest_id)) == -1){
                    perror("Writing Error");
                    exit(1);
                }
            }
            if(id_received >= MAX_IDS){
                break;
            }
        }
        close(server_socket);
        break;
    }

    close(sockfd_recv);
    printf("Exiting Server");
    unlink(socket_path);
    exit(0);
}