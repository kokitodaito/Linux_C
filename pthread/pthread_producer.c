/*生产者消费者实验*/
//使用信号量+管道模拟

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <errno.h>
#include <semaphore.h>
#include <sys/ipc.h>

#define MYFIFO "myfifo"
#define BUFFER_SIZE 3
#define UNIT_SIZE 5
#define RUN_TIME 30
#define DELAY_TIME_LEVELS 5.0

int fd;
time_t end_time;
sem_t mutex, full, avail;

/*producer*/
void *producer(void *arg)
{
    int read_write;
    int delay_time = 0;
    
    while(time(NULL) < end_time)
    {
        delay_time = (int)(rand()* DELAY_TIME_LEVELS/(RAND_MAX)/2.0) + 1;
        sleep(delay_time);
        /*P操作信号量avail和mutex*/
        sem_wait(&avail);
        sem_wait(&mutex);
        printf("\nProducer: delay = %d\n", delay_time);
        /*生产者写入数据*/
        if ((real_write = write(fd, "hello", UNIT_SIZE)) == -1)
        {
            printf("error occured\n");
        }
        else
        {
            printf("Write %d to the FIFO\n", read_write);
        }
        
        /*V操作信号量full和mutex*/
        sem_post(&full);
        sem_post(&mutex);
    }
    pthread_exit(NULL);
}

/*customer*/
void *customer(void *arg)
{
    unsigned char read_buffer[UNIT_SIZE];
    int real_read;
    int delay_time;

    while(time(NULL) < end_time)
    {
        delay_time = (int)(rand()* DELAY_TIME_LEVELS/(RAND_MAX)) + 1;
        sleep(delay_time);

        sem_wait(&full);
        sem_wait(&mutex);
        memset(read_buffer, 0, UNIT_SIZE);
        printf("\nCustomer: delay = %d\n", delay_time);

        if ((real_read = read(fd, read_buffer, UNIT_SIZE)) == -1)
        {
            printf("error occured\n");
        }
        else
        {
            printf("Read %s from FIFO\n", read_buffer);
        }

        sem_post(&avail);
        sem_post(&mutex);
    }
    pthread_exit(NULL);
}

int main()
{
    pthread_t thrd_prd_id, thrd_cst_id;
    pthread_t mon_th_id;
    int ret;

    srand(time(NULL));
    end_time = time(NULL) + RUN_TIME;

    if (mkfifo(MYFIFO, O_CREAT|O_EXCL) < 0)
    {
        printf("error occured\n");
    }

    fd = open(MYFIFO, O_RDWR);
    if (fd == -1)
    {
        printf("Open fifo error\n");
        return fd;
    }

    /*初始化信号量*/
    ret = sem_init(&mutex, 0, 1);
    ret += sem_init(&avail, 0, BUFFER_SIZE);
    ret += sem_init(&full, 0, 0);

    if (ret != 0)
    {
        printf("sem init failed\n");
        return ret;
    }

    /*创建线程*/
    ret = pthread_create(&thrd_prd_id, NULL, producer, NULL);
    if (ret != 0)
    {
        printf("Create producer pthread failed\n");
        return ret;
    }

    ret = pthread_create(&thrd_cst_id, NULL, customer, NULL);
    if (ret != 0)
    {
        printf("Create customer pthread failed\n");
        return ret;
    }

    pthread_join(thrd_prd_id, NULL);
    pthread_join(thrd_cst_id, NULL);
    close(fd);
    unlink(MYFIFO);
    return 0;
}