/*利用互斥锁同步的多线程程序 顺序执行*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREAD_NUMBER 3
#define REPEAT_NUMBER 3
#define DELAY_TIME_LEVELS 10.0
pthread_mutex_t mutex;

void *thrd_func(void *arg)
{
    int thrd_num = (int)arg;
    int delay_time = 0, count = 0;
    int res;
    /*互斥上锁*/
    res = pthread_mutex_lock(&mutex);
    if (res)
    {
        printf("Thread %d lock failed\n", thrd_num);
        pthread_exit(NULL);
    }
    printf("Thread %d is starting\n",thrd_num);
    for (count = 0; count < REPEAT_NUMBER; count++)
    {
        delay_time = (int)(rand()*DELAY_TIME_LEVELS/(RAND_MAX)) + 1;
        sleep(delay_time);
        printf("\tThread %d: job %d delay = %d\n", thrd_num, count, delay_time);
    }
    printf("Thread %d finished\n",thrd_num);
    pthread_exit(NULL);
}

int main()
{
    pthread_t thread[THREAD_NUMBER];
    int no = 0, res;
    void *thrd_ret;

    srand(time(NULL));

    /*互斥锁初始化*/
    pthread_mutex_init(&mutex,NULL);
    for (no = 0;no < THREAD_NUMBER;no++)
    {
        res = pthread_create(&thread[no],NULL,thrd_func,(void*)no);
        if (res != 0)
        {
            printf("Create thread %d failed\n", no);
            exit(res);
        }
    }
    printf("Create treads success\n Waiting for threads to finish ...\n");
    for (no = 0;no < THREAD_NUMBER;no++)
    {
        res = pthread_join(thread[no], &thrd_ret);
        if(!res)
        {
            printf("Thread %d joined successful\n",no);
        }
        else
        {
            printf("Thread %d join failed\n",no);
        }
        /*互斥锁解锁*/
        pthread_mutex_unlock(&mutex);
    }
    pthread_mutex_destroy(&mutex);
    return 0;
}