#include <assert.h>
#include <stdio.h>
#include "flight_control.h"

static void test_pid_positive_error(void)
{
    PidController pid;
    float out;

    pid_init(&pid, 0.1f, 0.0f, 0.0f, -100.0f, 100.0f);
    out = pid_update(&pid, 1500.0f, 1200.0f, 0.02f);

    assert(out > 0.0f);
}

static void test_pid_zero_error(void)
{
    PidController pid;
    float out;

    pid_init(&pid, 0.1f, 0.0f, 0.0f, -100.0f, 100.0f);
    out = pid_update(&pid, 1000.0f, 1000.0f, 0.02f);

    assert(out == 0.0f);
}

int main(void)
{
    test_pid_positive_error();
    test_pid_zero_error();

    printf("test_flight_control: PASS\n");
    return 0;
}