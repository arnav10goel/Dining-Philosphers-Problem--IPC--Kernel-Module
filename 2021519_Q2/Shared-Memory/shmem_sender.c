#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
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

    key_t mykeys[5] = {(key_t)100, (key_t)101, (key_t)102, (key_t)103, (key_t)104}; // 5 keys for 5 shared memory segments

    char* shared_memory[5]; //array of pointers to five shared memory segments

    int id_sent = 0;
    int id_received = 0;

    while(1){
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
            strcpy(shared_memory[i], listofstrings[id_sent]);
            printf("ID: %d String sent by Client is: %s\n", id_sent, shared_memory[i]);
            id_sent++;
            shmdt(shared_memory[i]); //detaching shared memory segment
            
        }
        pid_t pid = fork();
        if(pid < 0){
            perror("fork");
            exit(1);
        }
        else if(pid == 0){
            execl("./shmem_receiver", "shmem_receiver", NULL);
        }
        else{
            wait(NULL);
            printf("Highest ID by Client is: %d\n", id_sent);
        }
        if(id_sent == MAX_IDS){
            printf("All IDs sent and received\n");
            break;
        }
    }
    if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) {
      perror( "clock gettime" );
      exit( EXIT_FAILURE );
    }

    double res;

    res = (stop.tv_sec - start.tv_sec) + ( stop.tv_nsec - start.tv_nsec ) / 1000000000.0;
    
    printf("Time taken by Shared Memory IPC: %f seconds\n", res);
    return 0;
}