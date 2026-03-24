#include <stdio.h>
#include <windows.h>
#include "message_queue.h"



MessageQueue sensor_to_fc;
MessageQueue fc_to_act;

DWORD WINAPI sensor_task(LPVOID arg)
{

    while (1)
    {
        int altitude = 1200;

        queue_push(&sensor_to_fc, altitude);

        printf("[Sensor] Altitude = %d ft\n", altitude);

        Sleep(100);
    }
}

DWORD WINAPI flight_control_task(LPVOID arg)
{
    while (1)
    {
        int altitude;
        if (queue_pop(&sensor_to_fc, &altitude) == 0)
        {
            int target = 1500;
            int error = target - altitude;

            int command = error / 4;

            queue_push(&fc_to_act, command);

            printf("[Control] Error = %d, Cmd = %d\n", error, command);
        }

        Sleep(50);
    }
}

DWORD WINAPI actuator_task(LPVOID arg)
{
    while (1)
    {
        int cmd;
        if (queue_pop(&fc_to_act, &cmd) == 0)
        {
            printf("[Actuator] Elevator = %d\n", cmd);
        }

        Sleep(100);
    }
}

int main()
{
    HANDLE t1, t2, t3;

    queue_init(&sensor_to_fc);
    queue_init(&fc_to_act);

    t1 = CreateThread(NULL, 0, sensor_task, NULL, 0, NULL);
    t2 = CreateThread(NULL, 0, flight_control_task, NULL, 0, NULL);
    t3 = CreateThread(NULL, 0, actuator_task, NULL, 0, NULL);

    // Let the system run for some time
    #define RUN_TIME_MS 3000
    Sleep(RUN_TIME_MS);

    printf("Simulation finished.\n");

    // WaitForSingleObject(t1, INFINITE); //infinite simulation loop
    

    return 0;
}