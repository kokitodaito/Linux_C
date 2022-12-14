/*增加线程属性操作，设置分离线程*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define REPEAT_NUMBER 3
#define DELAY_TIME_LEVELS 10.0

int finish_flag = 0;

void *thrd_func(void *arg)
{
    int delay_time = 0;
    int count = 0;

    printf("Thread is starting\n");
    for (count = 0;count < REPEAT_NUMBER; count++)
    {
        delay_time = (int)(rand() * DELAY_TIME_LEVELS/(RAND_MAX)) + 1;
        sleep(delay_time);
        printf("\tThread : job %d delay = %d\n", count, delay_time);
    }

    printf("Thread finished\n");
    finish_flag = 1;
    pthread_exit(NULL);
}

int main()
{
    pthread_t thread;
    pthread_attr_t attr;
    int no = 0, res;
    void *thrd_ret;

    srand(time(NULL));

    /*初始化线程对象*/
    res = pthread_attr_init(&attr);
    if (res != 0)
    {
        printf("Create attr failed\n");
        exit(res);
    }

    res = pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
    res += pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (res != 0)
    {
        printf("Attr set falied\n");
        exit(res);
    }

    res = pthread_create(&thread, &attr, thrd_func, NULL);
    if (res != 0)
    {
        printf("Create thread failed\n");
        exit(res);
    }

    pthread_attr_destroy(&attr);
    printf("Create thread success\n");

    while(!finish_flag)
    {
        printf("Waiting for thread to finish\n");
        sleep(1);
    }
    return 0;
}