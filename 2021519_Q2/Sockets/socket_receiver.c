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
    unsigned int sockfd_recv;
    struct sockaddr_un string_receiver;
    memset(&string_receiver, 0, sizeof(string_receiver));
    int len, server_socket;

    sockfd_recv = socket(AF_UNIX, SOCK_STREAM, 0);
    if(sockfd_recv == -1)
    {
        perror("Socket Creation Failed");
        exit(1);
    }
    string_receiver.sun_family = AF_UNIX;
    char* socket_path = "mysocket.socket";
    strcpy(string_receiver.sun_path, socket_path);
    unlink(string_receiver.sun_path);

    if(connect(sockfd_recv, (struct sockaddr *)&string_receiver, sizeof(string_receiver)) == -1)
    {
        perror("Binding Error");
        exit(1);
    }


    int id_received = 0;
    int highest_id = 0; 
    char string_received[STR_LEN+1];

    while(1){
        while(1){
            if(read(sockfd_recv, string_received, STR_LEN+1) == -1)
            {
                perror("Reading Error");
                exit(1);
            }
            printf("ID Received: %d String Received: %s\n", id_received, string_received);
            id_received++;
        }
        close(server_socket);
    }

    close(sockfd_recv);
    return 0;
}