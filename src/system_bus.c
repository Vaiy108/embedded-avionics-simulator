#include "system_bus.h"

#define SYSTEM_BUS_SIZE 16

static SystemMessage queue[SYSTEM_BUS_SIZE];
static int head = 0;
static int tail = 0;
static int count = 0;

void system_bus_init(void)
{
    head = 0;
    tail = 0;
    count = 0;
}

StatusCode system_bus_publish(SystemMessage msg)
{
    if (count >= SYSTEM_BUS_SIZE)
    {
        return STATUS_QUEUE_FULL;
    }

    queue[tail] = msg;
    tail = (tail + 1) % SYSTEM_BUS_SIZE;
    count++;

    return STATUS_OK;
}

StatusCode system_bus_receive(SystemMessage *msg)
{
    if (msg == 0)
    {
        return STATUS_INVALID_PARAM;
    }

    if (count == 0)
    {
        return STATUS_QUEUE_EMPTY;
    }

    *msg = queue[head];
    head = (head + 1) % SYSTEM_BUS_SIZE;
    count--;

    return STATUS_OK;
}