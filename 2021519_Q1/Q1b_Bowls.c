#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>

sem_t forks[5];
sem_t bowl; // Intitialized to 2 at sem_init
int phil0, phil1, phil2, phil3, phil4, phil5;

void* philospher(void* num){
    int philopsher_num = *((int*)num);
    int right_fork, left_fork;

    printf("Philosopher %d is here\n", philopsher_num);
    sleep(1);

    int t = 1000;

    while(t > 0){
        printf("Philosopher %d is thinking\n", philopsher_num);
        sleep(1);
        printf("Philosopher %d is hungry\n", philopsher_num);
        sem_wait(&bowl);
        printf("Philosopher %d picked up bowl\n", philopsher_num);
        if(philopsher_num == 0){
            right_fork = philopsher_num;
            left_fork = (philopsher_num + 1) % 5;
        }
        else{
            right_fork = (philopsher_num + 1) % 5;
            left_fork = philopsher_num;
        }
        sem_wait(&forks[right_fork]);
        printf("Philosopher %d picked up right fork %d\n", philopsher_num, right_fork);
        sem_wait(&forks[left_fork]);
        printf("Philosopher %d picked up left fork %d\n", philopsher_num, left_fork);
        printf("Philosopher %d is eating\n", philopsher_num);
        if(philopsher_num == 0){
            phil0 += 1;
        }
        else if(philopsher_num == 1){
            phil1 += 1;
        }
        else if(philopsher_num == 2){
            phil2 += 1;
        }
        else if(philopsher_num == 3){
            phil3 += 1;
        }
        else if(philopsher_num == 4){
            phil4 += 1;
        }
        sleep(1);
        sem_post(&forks[right_fork]);
        printf("Philosopher %d put down right fork %d\n", philopsher_num, right_fork);
        sem_post(&forks[left_fork]);
        printf("Philosopher %d put down left fork %d\n", philopsher_num, left_fork);
        sem_post(&bowl);
        printf("Philosopher %d has left the bowl\n", philopsher_num);
        t -= 1;
    }

    return NULL;
}

int main(){
    pthread_t philosophers[5];
    int ids[5];

    phil0 = 0;
    phil1 = 0;
    phil2 = 0;
    phil3 = 0;
    phil4 = 0;

    sem_init(&bowl, 0, 2);
    for(int i = 0; i < 5; i++){
        sem_init(&forks[i], 0, 1);
    }

    for(int i = 0; i < 5; i++){
        ids[i] = i;
        pthread_create(&philosophers[i], NULL, &philospher, &ids[i]);
    }

    for(int i = 0; i < 5; i++){
        pthread_join(philosophers[i], NULL);
    }

    for(int i = 0; i < 5; i++){
        sem_destroy(&forks[i]);
    }
    sem_destroy(&bowl);

    printf("Philosopher 0 ate %d times\n", phil0);
    printf("Philosopher 1 ate %d times\n", phil1);
    printf("Philosopher 2 ate %d times\n", phil2);
    printf("Philosopher 3 ate %d times\n", phil3);
    printf("Philosopher 4 ate %d times\n", phil4);

    return 0;

}