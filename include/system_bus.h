#ifndef SYSTEM_BUS_H
#define SYSTEM_BUS_H

#include <stdint.h>
#include "platform_types.h"

typedef enum
{
    MSG_ALTITUDE = 1,
    MSG_ELEVATOR_CMD
} SystemMessageType;

typedef struct
{
    SystemMessageType type;
    int32_t value;
} SystemMessage;

void system_bus_init(void);
StatusCode system_bus_publish(SystemMessage msg);
StatusCode system_bus_receive(SystemMessage *msg);

#endif