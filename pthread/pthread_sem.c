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
    int no = 0,res;
    void *thrd_ret;

    srand(time(NULL));
    for (no = 0;no < THREAD_NUMBER;no++)
    {
        /*信号量初始化为0*/
        sem_init(&sem[no],0,0);
        res = pthread_create(&thread[no], NULL, thrd_func, (void*)no);
        if (res != 0)
        {
            printf("Create thread %d failed\n", no);
            exit(res);
        }
    }

    printf("Create threads success\n Waiting for threads to finish\n");

    /*对第一个创建的线程执行V操作*/
    sem_post(&sem[0]);
    for (no = 0;no < THREAD_NUMBER-1;no++)
    {
        res = pthread_join(thread[no], &thrd_ret);
        if (!res)
        {
            printf("Thread %d joined\n",no);
        }
        else
        {
            printf("Thread %d join failed\n",no);
        }
        /*V操作*/
        sem_post(&sem[no+1]);
    }

    pthread_join(thread[2],&thrd_ret);

    for (no=0;no<THREAD_NUMBER;no++)
    {
        sem_destroy(&sem[no]);
    }

    return 0;
}