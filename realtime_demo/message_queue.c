#include "message_queue.h"

void queue_init(MessageQueue *q)
{
    q->head = 0;
    q->tail = 0;
    q->count = 0;
}

int queue_push(MessageQueue *q, int32_t value)
{
    if (q->count >= QUEUE_SIZE)
        return -1;

    q->data[q->tail] = value;
    q->tail = (q->tail + 1) % QUEUE_SIZE;
    q->count++;
    return 0;
}

int queue_pop(MessageQueue *q, int32_t *value)
{
    if (q->count == 0)
        return -1;

    *value = q->data[q->head];
    q->head = (q->head + 1) % QUEUE_SIZE;
    q->count--;
    return 0;
}