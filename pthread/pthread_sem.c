/*利用信号量同步的多线程 顺序执行*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define THREAD_NUMBER 3
#define REPEAT_NUMBER 3
#define DELAY_TIME_LEVELS 10.0
sem_t sem[THREAD_NUMBER];


void *thrd_func(void *arg)
{
    int thrd_num = (int)arg;
    int delay_time = 0;
    int count = 0;
    /*进行P操作*/
    sem_wait(&sem[thrd_num]);
    printf("Thread %d is starting\n",thrd_num);

    for (count = 0;count < REPEAT_NUMBER;count++)
    {
        delay_time = (int)(rand() * DELAY_TIME_LEVELS/(RAND_MAX)) + 1;
        sleep(delay_time);
        printf("\tThread %d: job %d delay = %d\n", thrd_num, count, delay_time);
    }

    printf("Thread %d finished\n", thrd_num);
    pthread_exit(NULL);
}

int main(void)
{
    pthread_t thread[THREAD_NUMBER];
    
}