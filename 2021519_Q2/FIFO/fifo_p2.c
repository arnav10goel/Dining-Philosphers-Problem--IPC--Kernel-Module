#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>
#include<fcntl.h>

#define MAX_IDS 50

int main(){
    int fd2;
    char *myfifo = "/tmp/myfifo";
    mkfifo(myfifo, 0666);

    int id_received = 0;
    int highest_id = 0;
    while(1){
        char strs_received[5][6];   // 5 strings of length 5
        for(int i = 0; i < 5; i++){
            fd2 = open(myfifo, O_RDONLY);
            if(fd2 == -1){
                perror("open");
                exit(1);
            }
            if(read(fd2, strs_received[i], 6*sizeof(char)) == -1){
                perror("read");
                exit(1);
            }
            close(fd2);
            id_received += 1;
            strs_received[i][5] = '\0';
            char* str = (malloc)(6*sizeof(char));
            str = strs_received[i];
            printf("ID: %d Received string: %s\n", id_received, str);

        }

        fd2 = open(myfifo, O_WRONLY);
        if(fd2 == -1){
            perror("open");
            exit(1);
        }
        highest_id += 5;
        if(write(fd2, &highest_id, sizeof(int)) == -1){
            perror("write");
            exit(1);
        }
        close(fd2);
        printf("Highest ID sent: %d\n", highest_id);
        if(highest_id > MAX_IDS - 1){
            break;
        }
    }
    unlink(myfifo);
    return 0;
        
}