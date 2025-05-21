#ifndef __COM_PID_H__
#define __COM_PID_H__

#include "main.h"

int Com_PID_Angle(float kp, float kd, float kalman_angle, float balace_angle, int16_t gy);

int Com_PID_Velocity(float kp, float ki, int16_t encoder_a, int16_t encoder_b, int16_t move_distance);

int Com_PID_Turn(float kp, short gz);

#endif /* __COM_PID_H__ */
