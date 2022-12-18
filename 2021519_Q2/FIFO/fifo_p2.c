#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>
#include<fcntl.h>

#define MAX_IDS 50
#define STR_LEN 6

int main(){

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

    int id_received = 0;
    int highest_id = 0; 
    char string_received[STR_LEN+1];

    while(1){
        for(int i = 0; i < 5; i++){
            fd = open(myfifo, O_RDONLY);
            if(fd == -1){
                perror("open");
                exit(1);
            }
            if(read(fd, string_received, sizeof(string_received)) == -1){
                perror("read");
                exit(1);
            }
            close(fd);
            char* str = (malloc)(7*sizeof(char));
            str = string_received;
            printf("ID: %d Received string: %s\n", id_received+1, str);
            id_received += 1;
        }

        fd2 = open(myfifo2, O_WRONLY);
        if(fd2 == -1){
            perror("open");
            exit(1);
        }
        
        highest_id = id_received;
        if(write(fd2, &highest_id, sizeof(int)) == -1){
            perror("write");
            exit(1);
        }
        close(fd2);
        printf("Highest ID sent: %d\n", highest_id);
        if(highest_id >= MAX_IDS){
            exit(0);
        }
    }
    unlink(myfifo);
    unlink(myfifo2);
    return 0;
        
}