#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "buffer.h"
int head, tail;


buffer_item buffer[BUFFER_SIZE+1];

int insert_item(buffer_item item) {
    // insert item into buffer
    // return 0 if successful, otherwise
    // return -1 indicating an error condition
    if ((tail+1)%(BUFFER_SIZE+1) != head)
    {
        tail = (tail+1)%(BUFFER_SIZE+1);
        buffer[tail] = item;  
        return 0;
    }
    else
    {
        return -1;
    }
}

int remove_item(buffer_item *item) {
    // remove an object from buffer
    // placing it in item
    // return 0 if successful, otherwise
    // return -1 indicating an error condition
    if (head == tail)
    {
        return -1;
    }
    head = (head+1)%(BUFFER_SIZE+1);
    *item = buffer[head];
    return 0;
}

void buffer_init() {
    // initialize buffer
    // return 0 if successful, otherwise
    // return -1 indicating an error condition
    head = 0;
    tail = 0;
}
