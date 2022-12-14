/*多线程举例 无同步操作 各线程独立运行*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREAD_NUMBER 3
#define REPEAT_NUMBER 5
#define DELAY_TIME_LEVELS 10

void *thrd_func(void *arg)
{
    int thrd_num = (int)arg;
    int delay_time = 0;
    int count = 0;

    printf("Thread %d is starting\n", thrd_num);
    for(count = 0;count < REPEAT_NUMBER;count ++)
    {
        delay_time = (int)(rand()*DELAY_TIME_LEVELS/(RAND_MAX)) + 1;
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
    void* thrd_ret;

    /*
    设置随机数种子 计算机无法生成随机数 会把无规则数字分份存储 srand根据参数指向不同的无规则数据
    使用time(NULL)获得当前时间 因为时间不断变动 所以srand参数变化 rand取得值也随之变化 从而模拟随机数
    */
    srand(time(NULL));

    for(no = 0;no < THREAD_NUMBER;no++)
    {
        res = pthread_create(&thread[no], NULL, thrd_func, (void*)no);
        if (res != 0)
        {
            printf("Create thread %d failed\n",no);
            exit(res);
        }
    }

    printf("Create threads success\n Waiting for threads to finish...\n");
    for (no = 0;no < THREAD_NUMBER;no++)
    {
        res = pthread_join(thread[no], &thrd_ret);
        if (!res)
        {
            printf("Thread %d joined\n",no);
        }
        else
        {
            printf("Thread %d failed\n",no);
        }
    }
    return 0;
}