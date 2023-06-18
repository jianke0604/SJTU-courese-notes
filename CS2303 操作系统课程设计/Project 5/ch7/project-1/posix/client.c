/**
 * Example client program that uses thread pool.
 */

#include <stdio.h>
#include <unistd.h>
#include "threadpool.h"
#define NUMBER_OF_TASKS 20

struct data
{
    int a;
    int b;
};

void add(void *param)
{
    struct data *temp;
    temp = (struct data*)param;

    printf("I add two values %d and %d result = %d\n",temp->a, temp->b, temp->a + temp->b);
}

int main(void)
{
    // create some work to do
    struct data work[NUMBER_OF_TASKS];
    for(int i=0; i<NUMBER_OF_TASKS; i++)
    {
        work[i].a = rand()%100;
        work[i].b = rand()%100;
    }
    printf("work created\n");
    // initialize the thread pool
    pool_init();
    printf("pool initialized\n");
    // submit the work to the queue
    // pool_submit(&add,&work);
    for (int i = 0; i < NUMBER_OF_TASKS; i++)
    {
        pool_submit(&add,&work[i]);
    }

    // may be helpful 
    sleep(3);

    pool_shutdown();

    return 0;
}
