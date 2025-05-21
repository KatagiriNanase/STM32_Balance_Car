#ifndef __INT_TB6612_H__
#define __INT_TB6612_H__

#include "main.h"

#define MOTOR_FORWARD 0
#define MOTOR_BACK 1
#define MOTOR_BRAKE 2

/* AIN1,AIN2拉高拉低宏定义 */
#define AIN1_HIGH HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_SET)
#define AIN1_LOW HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_RESET)
#define AIN2_HIGH HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_SET)
#define AIN2_LOW HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_RESET)

/* BIN1,BIN2拉高拉低宏定义 */
#define BIN1_HIGH HAL_GPIO_WritePin(BIN1_GPIO_Port,BIN1_Pin,GPIO_PIN_SET)
#define BIN1_LOW HAL_GPIO_WritePin(BIN1_GPIO_Port,BIN1_Pin,GPIO_PIN_RESET)
#define BIN2_HIGH HAL_GPIO_WritePin(BIN2_GPIO_Port,BIN2_Pin,GPIO_PIN_SET)
#define BIN2_LOW HAL_GPIO_WritePin(BIN2_GPIO_Port,BIN2_Pin,GPIO_PIN_RESET)

/**
 *@brief 初始化电机
 * 
 */
void Int_TB6612_Init(void);

/**
 *@brief 根据带符号的pwm值，设置转速和方向
 *
 * @param pwma  A电机带符号的占空比
 * @param pwmb  B电机带符号的占空比
 */
void Int_TB6612_SetPWM(int pwma, int pwmb);

#endif /* __INT_TB6612_H__ */
