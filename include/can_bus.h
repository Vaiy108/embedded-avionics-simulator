#ifndef CAN_BUS_H
#define CAN_BUS_H

#include <stdint.h>
#include "platform_types.h"

#define CAN_MAX_DATA_LEN 8
#define CAN_QUEUE_SIZE 16

typedef struct
{
    uint32_t id;
    uint8_t dlc;
    uint8_t data[CAN_MAX_DATA_LEN];
} CanFrame;

typedef struct
{
    CanFrame queue[CAN_QUEUE_SIZE];
    int head;
    int tail;
    int count;
} CanBus;

void can_bus_init(CanBus *bus);
StatusCode can_bus_send(CanBus *bus, const CanFrame *frame);
StatusCode can_bus_receive(CanBus *bus, CanFrame *frame_out);

#endif