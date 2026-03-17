#include <assert.h>
#include <stdio.h>
#include "can_bus.h"
#include "platform_types.h"

static void test_can_send_receive(void)
{
    CanBus bus;
    CanFrame tx;
    CanFrame rx;
    StatusCode status;

    can_bus_init(&bus);

    tx.id = 0x101;
    tx.dlc = 2;
    tx.data[0] = 0x34;
    tx.data[1] = 0x12;

    status = can_bus_send(&bus, &tx);
    assert(status == STATUS_OK);

    status = can_bus_receive(&bus, &rx);
    assert(status == STATUS_OK);

    assert(rx.id == 0x101);
    assert(rx.dlc == 2);
    assert(rx.data[0] == 0x34);
    assert(rx.data[1] == 0x12);
}

static void test_can_receive_empty_queue(void)
{
    CanBus bus;
    CanFrame rx;
    StatusCode status;

    can_bus_init(&bus);
    status = can_bus_receive(&bus, &rx);

    assert(status == STATUS_QUEUE_EMPTY);
}

int main(void)
{
    test_can_send_receive();
    test_can_receive_empty_queue();

    printf("test_can: PASS\n");
    return 0;
}