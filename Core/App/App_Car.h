#ifndef __APP_CAR_H__
#define __APP_CAR_H__

#include "Int_MPU6050.h"
#include "Com_Filter.h"
#include <math.h>
#include "adc.h"
#include "OLED.h"
#include <stdio.h>
#include "Int_Encoder.h"

#define PI 3.14159265

extern _Bool flag_up, flag_down, flag_left, flag_right;

/**
 * @brief 计算小车倾斜角度
 *
 */
void App_Car_GetAngle(void);

/**
 *@brief OLED显示电池电压、编码器值、倾角值
 *
 */
void App_Car_Display(void);

void App_Car_PID(void);
#endif /* __APP_CAR_H__ */
