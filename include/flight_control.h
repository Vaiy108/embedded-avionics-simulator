#ifndef FLIGHT_CONTROL_H
#define FLIGHT_CONTROL_H

typedef struct
{
    float kp;
    float ki;
    float kd;
    float integral;
    float prev_error;
    float output_min;
    float output_max;
} PidController;

void pid_init(PidController *pid, float kp, float ki, float kd, float output_min, float output_max);
float pid_update(PidController *pid, float setpoint, float measurement, float dt_sec);

#endif