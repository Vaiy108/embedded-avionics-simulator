#include "can_bus.h"

/**
 * @brief Initialize simulated CAN bus queue.
 * @req AVSIM_CAN_001
 */
void can_bus_init(CanBus *bus)
{
    if (bus == 0)
    {
        return;
    }

    bus->head = 0;
    bus->tail = 0;
    bus->count = 0;
}

/**
 * @brief Queue a CAN frame for transmission.
 * @req AVSIM_CAN_002
 */
StatusCode can_bus_send(CanBus *bus, const CanFrame *frame)
{
    if ((bus == 0) || (frame == 0))
    {
        return STATUS_INVALID_PARAM;
    }

    if (frame->dlc > CAN_MAX_DATA_LEN)
    {
        return STATUS_INVALID_PARAM;
    }

    if (bus->count >= CAN_QUEUE_SIZE)
    {
        return STATUS_QUEUE_FULL;
    }

    bus->queue[bus->tail] = *frame;
    bus->tail = (bus->tail + 1) % CAN_QUEUE_SIZE;
    bus->count++;

    return STATUS_OK;
}

/**
 * @brief Receive a CAN frame from simulated queue.
 * @req AVSIM_CAN_003
 */
StatusCode can_bus_receive(CanBus *bus, CanFrame *frame_out)
{
    if ((bus == 0) || (frame_out == 0))
    {
        return STATUS_INVALID_PARAM;
    }

    if (bus->count == 0)
    {
        return STATUS_QUEUE_EMPTY;
    }

    *frame_out = bus->queue[bus->head];
    bus->head = (bus->head + 1) % CAN_QUEUE_SIZE;
    bus->count--;

    return STATUS_OK;
}