#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>

#define MAX_IDS 50
#define STR_LEN 6

int main(int argc, char* argv[]){
    key_t mykeys[5] = {(key_t)100, (key_t)101, (key_t)102, (key_t)103, (key_t)104}; // 5 keys for 5 shared memory segments

    char* shared_memory[5]; //array of pointers to five shared memory segments
    printf("Receiver/Server process started\n");
    for(int i = 0; i < 5; i++){
        int shmid = shmget(mykeys[i], 1024, IPC_CREAT | 0666); //creating shared memory segment
        if(shmid == -1){
            perror("shmget");
            exit(1);
        }
        shared_memory[i] = (char*)shmat(shmid, NULL, 0); //attaching shared memory segment
        if(shared_memory[i] == (char*)-1){
            perror("shmat");
            exit(1);
        }
        printf("String received is: %s\n", shared_memory[i]);
    }
    printf("Receiver/Server process ended\n");
}