#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include <stdint.h>

#define QUEUE_SIZE 10

typedef struct
{
    int32_t data[QUEUE_SIZE];
    int head;
    int tail;
    int count;
} MessageQueue;

void queue_init(MessageQueue *q);
int queue_push(MessageQueue *q, int32_t value);
int queue_pop(MessageQueue *q, int32_t *value);

#endif