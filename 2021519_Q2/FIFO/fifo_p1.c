#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<errno.h>
#include<fcntl.h>
#include<pthread.h>

#define MAX_IDS 50
#define STR_LEN 6

int main(){

    struct timespec start, stop;
    if( clock_gettime(CLOCK_REALTIME, &start) == -1 ) {
      perror( "clock gettime" );
      exit( EXIT_FAILURE );
    }
    //Code snippet to generate 50 random strings of length 5
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

    //Creating 2 FIFOs
    int fd, fd2; //file descriptor
    char *myfifo = "myfifo";
    if(mkfifo(myfifo, 0666) == -1){
        if(errno != EEXIST){
            perror("mkfifo");
            exit(1);
        }
    }
    char *myfifo2 = "myfifo2";
    if(mkfifo(myfifo2, 0666) == -1){
        if(errno != EEXIST){
            perror("mkfifo2");
            exit(1);
        }
    }

    int id_sent = 0;
    int id_received = 0;
    while(1){
        for(int j = 0; j < 5; j++){
            fd = open(myfifo, O_WRONLY);
            if(fd == -1){
                perror("open");
                exit(1);
            }
            if(write(fd, listofstrings[id_sent], strlen(listofstrings[id_sent])+1) == -1){
                perror("write");
                exit(1);
            }
            id_sent += 1;
            close(fd);
            usleep(1);
        }

        fd = open(myfifo2, O_RDONLY);
        if(fd == -1){
            perror("open");
            exit(1);
        }
        
        if(read(fd, &id_received, sizeof(int)) == -1){
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
    unlink(myfifo2);

    if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) {
      perror( "clock gettime" );
      exit( EXIT_FAILURE );
    }

    double res;
    res = (stop.tv_sec - start.tv_sec) + ( stop.tv_nsec - start.tv_nsec ) / 1000000000.0;
    printf("Time taken by FIFO IPC: %f seconds\n", res);
    return 0;
}
