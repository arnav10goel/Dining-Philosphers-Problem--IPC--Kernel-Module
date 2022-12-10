#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>
#include<fcntl.h>

#define MAX_IDS 50
#define STR_LEN 5

int main(){
    //Code snippet to generate 50 random strings of length 5
    char **listofstrings = malloc(50 * sizeof(char *));
    char listofalphabets[26][2] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k",
                                "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v",
                                "w", "x", "y", "z"};
    
    for(int i = 0; i < 50; i++){
        listofstrings[i] = malloc(STR_LEN * sizeof(char));
        for(int j = 0; j < STR_LEN; j++){
            listofstrings[i][j] = listofalphabets[rand()%26][0];
        }    
    }

    //Creating a FIFO
    int fd; //file descriptor
    char *myfifo = "/tmp/myfifo";
    if(mkfifo(myfifo, 0666) == -1){
        if(errno != EEXIST){
            perror("mkfifo");
            exit(1);
        }
    }
    int id_sent = 0;
    while(1){
        fd = open(myfifo, O_WRONLY);
        if(fd == -1){
            perror("open");
            exit(1);
        }
        for(int j = 0; j < 5; j++){
            id_sent += j;
            if(write(fd, &id_sent, sizeof(id_sent)) == -1){
                perror("write");
                //exit(1);
            }

            if(write(fd, listofstrings[id_sent+j], STR_LEN) == -1){
                perror("write");
                //exit(1);
            }
        }
        close(fd);

        fd = open(myfifo, O_RDONLY);
        if(fd == -1){
            perror("open");
            exit(1);
        }
        
        if(read(fd, &id_sent, sizeof(int)) == -1){
            perror("read");
            exit(1);
        }
        printf("Highest ID received from P2: %d\n", id_sent);
        close(fd);
        if(id_sent > MAX_IDS-1){
            printf("All strings sent and received\n");
            break;
        }

    }
    unlink(myfifo);
}
