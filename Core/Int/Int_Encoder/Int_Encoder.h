#ifndef __INT_ENCODER_H__
#define __INT_ENCODER_H__

#include "main.h"

/* 电机A,B宏定义 */
#define MOTOR_A 1
#define MOTOR_B 2

/**
 *@brief 初始化编码器
 *
 */
void Int_Encoder_Init(void);

/**
 *@brief 读取编码器模式的计数值，处理为有符号值
 *
 * @param htim 定时器x的句柄
 * @return int16_t 带方向的计数值
 */
int16_t Int_Encoder_ReadCounter(uint8_t motorx);


#endif /* __INT_ENCODER_H__ */
