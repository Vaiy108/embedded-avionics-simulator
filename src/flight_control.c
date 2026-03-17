#include "flight_control.h"

/**
 * @brief Clamp value to actuator limits.
 */
static float clamp(float value, float min_val, float max_val)
{
    if (value < min_val)
    {
        return min_val;
    }

    if (value > max_val)
    {
        return max_val;
    }

    return value;
}

void pid_init(PidController *pid, float kp, float ki, float kd, float output_min, float output_max)
{
    if (pid == 0)
    {
        return;
    }

    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->integral = 0.0f;
    pid->prev_error = 0.0f;
    pid->output_min = output_min;
    pid->output_max = output_max;
}

/**
 * @brief Compute elevator command for altitude hold.
 * @req AVSIM_FC_001
 */
float pid_update(PidController *pid, float setpoint, float measurement, float dt_sec)
{
    float error;
    float derivative;
    float output;

    if ((pid == 0) || (dt_sec <= 0.0f))
    {
        return 0.0f;
    }

    error = setpoint - measurement;
    pid->integral += error * dt_sec;
    derivative = (error - pid->prev_error) / dt_sec;

    output = (pid->kp * error) +
             (pid->ki * pid->integral) +
             (pid->kd * derivative);

    pid->prev_error = error;

    return clamp(output, pid->output_min, pid->output_max);
}