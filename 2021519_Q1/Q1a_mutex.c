#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

pthread_mutex_t forks[5];

int phil0, phil1, phil2, phil3, phil4, phil5;

void* philospher(void* num){
    int philopsher_num = *((int*)num);
    int right_fork, left_fork;

    printf("Philosopher %d is here\n", philopsher_num);
    //sleep(1);

    int t = 1000;

    while(t > 0){
        printf("Philosopher %d is thinking\n", philopsher_num);
        //sleep(1);
        printf("Philosopher %d is hungry\n", philopsher_num);
        right_fork = philopsher_num;
        left_fork = (philopsher_num + 1) % 5;
        pthread_mutex_lock(&forks[right_fork]);
        printf("Philosopher %d picked up right fork %d\n", philopsher_num, right_fork);
        pthread_mutex_lock(&forks[left_fork]);
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
        pthread_mutex_unlock(&forks[right_fork]);
        printf("Philosopher %d put down right fork %d\n", philopsher_num, right_fork);
        pthread_mutex_unlock(&forks[left_fork]);
        printf("Philosopher %d put down left fork %d\n", philopsher_num, left_fork);
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

    for(int i = 0; i < 5; i++){
        pthread_mutex_init(&forks[i], NULL);
    }

    for(int i = 0; i < 5; i++){
        ids[i] = i;
        pthread_create(&philosophers[i], NULL, &philospher, &ids[i]);
    }

    for(int i = 0; i < 5; i++){
        pthread_join(philosophers[i], NULL);
    }

    for(int i = 0; i < 5; i++){
        pthread_mutex_destroy(&forks[i]);
    }

    printf("Philosopher 0 ate %d times\n", phil0);
    printf("Philosopher 1 ate %d times\n", phil1);
    printf("Philosopher 2 ate %d times\n", phil2);
    printf("Philosopher 3 ate %d times\n", phil3);
    printf("Philosopher 4 ate %d times\n", phil4);

    return 0;

}