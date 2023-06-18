#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include "buffer.h"


sem_t empty, full;
pthread_mutex_t mutex;
int flag = 0;

void *producer(void *param)
{
    buffer_item item;
    while (1)
    {
        /* sleep for a random period of time */
        sleep(rand() % 5);
        /* generate a random number */
        item = rand()%100;
        sem_wait(&empty);
        if (flag)
            break;
        pthread_mutex_lock(&mutex);
        if (insert_item(item))
            fprintf(stderr, "report error condition");
        else
            printf("\033[1;32mproducer produced item %d\033[0m\n", item);
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
}

void *consumer(void *param)
{
    buffer_item item;
    while (1)
    {
        /* sleep for a random period of time */
        sleep(rand() % 5);
        sem_wait(&full);
        if (flag)
            break;
        pthread_mutex_lock(&mutex);
        if (remove_item(&item))
            fprintf(stderr, "report error condition");
        else
            printf("\033[1;31mconsumer consumed item %d\033[0m\n", item);
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}

int main(int argc, char *argv[])
{
    int sleepTime, producerThreads, consumerThreads;
    int i, j;
    /* 1. Get command line arguments argv[1],argv[2],argv[3] */
    if (argc != 4)
    {
        fprintf(stderr, "Usage: <sleep time> <producer threads> <consumer threads>\n");
        return -1;
    }
    sleepTime = atoi(argv[1]);
    producerThreads = atoi(argv[2]);
    consumerThreads = atoi(argv[3]);
    /* 2. Initialize buffer */
    buffer_init();
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);
    /* 3. Create producer thread(s) */
    for (i = 0; i < producerThreads; i++)
    {
        pthread_t tid;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&tid, &attr, producer, NULL);
    }
    /* 4. Create consumer thread(s) */
    for (j = 0; j < consumerThreads; j++)
    {
        pthread_t tid;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&tid, &attr, consumer, NULL);
    }
    /* 5. Sleep */
    sleep(sleepTime);
    /* 6. Exit */
    flag = 1;
    for (i = 0; i < producerThreads; i++)
    {
        sem_post(&empty);
    }
    for (j = 0; j < consumerThreads; j++)
    {
        sem_post(&full);
    }
    printf("Exit the program\n");
    exit(0);
}