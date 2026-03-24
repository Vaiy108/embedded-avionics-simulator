/**
 * @file main.c
 * @brief Main integration of avionics simulator.
 */

#include <stdio.h>
#include <stdint.h>
#include "platform_types.h"
#include "arinc429.h"
#include "can_bus.h"
#include "flight_control.h"
#include "sensor_sim.h"
#include "actuator.h"
#include "system_bus.h"

#define ARINC_LABEL_ALTITUDE 0x8B
#define CAN_ID_ELEVATOR_CMD  0x101

static void pack_int16_le(uint8_t data[8], int16_t value)
{
    data[0] = (uint8_t)(value & 0xFF);
    data[1] = (uint8_t)((value >> 8) & 0xFF);
}

static int16_t unpack_int16_le(const uint8_t data[8])
{
    return (int16_t)(((int16_t)data[1] << 8) | data[0]);
}

int main(void)
{
    int altitude_ft;
    uint32_t arinc_word;
    Arinc429Fields fields;
    StatusCode status;
    SystemMessage msg;
    PidController pid;
    float target_altitude_ft = 1500.0f;
    float elevator_cmd;
    CanBus bus;
    CanFrame tx;
    CanFrame rx;
    int16_t cmd;

    system_bus_init();

    altitude_ft = sensor_read_altitude_ft();

    arinc_word = arinc429_encode(ARINC_LABEL_ALTITUDE, 0, altitude_ft, 0);

    status = arinc429_decode(arinc_word, &fields);
    if (status != STATUS_OK)
    {
        printf("ARINC429 decode failed\n");
        return 1;
    }

    printf("Decoded altitude from ARINC429: %d ft\n", fields.data);

    msg.type = MSG_ALTITUDE;
    msg.value = fields.data;

    status = system_bus_publish(msg);
    if (status != STATUS_OK)
    {
        printf("System bus publish failed\n");
        return 1;
    }

    status = system_bus_receive(&msg);
    if ((status != STATUS_OK) || (msg.type != MSG_ALTITUDE))
    {
        printf("System bus receive failed\n");
        return 1;
    }

    printf("Target altitude: %.1f ft\n", target_altitude_ft);
    printf("Altitude error: %.1f ft\n", target_altitude_ft - (float)msg.value);

    pid_init(&pid, 0.02f, 0.001f, 0.005f, -100.0f, 100.0f);
    elevator_cmd = pid_update(&pid, target_altitude_ft, (float)msg.value, 0.02f);

    can_bus_init(&bus);

    tx.id = CAN_ID_ELEVATOR_CMD;
    tx.dlc = 2;
    tx.data[0] = 0;
    tx.data[1] = 0;
    tx.data[2] = 0;
    tx.data[3] = 0;
    tx.data[4] = 0;
    tx.data[5] = 0;
    tx.data[6] = 0;
    tx.data[7] = 0;

    pack_int16_le(tx.data, (int16_t)elevator_cmd);

    status = can_bus_send(&bus, &tx);
    if (status != STATUS_OK)
    {
        printf("CAN send failed\n");
        return 1;
    }

    status = can_bus_receive(&bus, &rx);
    if (status != STATUS_OK)
    {
        printf("CAN receive failed\n");
        return 1;
    }

    cmd = unpack_int16_le(rx.data);
    actuator_apply_elevator_command(cmd);

    return 0;
}