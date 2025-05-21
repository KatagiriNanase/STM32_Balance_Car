#include "Int_TB6612.h"
#include "tim.h"
#include <stdlib.h>

void Int_TB6612_Init(void)
{
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);

}



/**
 *@brief 控制电机A方向
 *
 * @param direct 转动的方向
 */
void Int_TB6612_MotorB(uint8_t direct)
{
    if (direct == MOTOR_FORWARD)
    {
        /* 正转 AIN1=0,AIN2=1 */
        AIN1_LOW;
        AIN2_HIGH;
    }
    else if (direct == MOTOR_BACK)
    {
        /* 反转 AIN1=1,AIN2=0 */
        AIN1_HIGH;
        AIN2_LOW;
    }
    else
    {
        /* 刹车 AIN=1 AIN2=1 */
        AIN1_HIGH;
        AIN2_HIGH;
    }
}

/**
 *@brief 控制电机B方向
 *
 * @param direct 转动的方向
 */
void Int_TB6612_MotorA(uint8_t direct)
{
    if (direct == MOTOR_FORWARD)
    {
        /* 正转 AIN1=0,AIN2=1 */
        BIN1_LOW;
        BIN2_HIGH;
    }
    else if (direct == MOTOR_BACK)
    {
        /* 反转 AIN1=1,AIN2=0 */
        BIN1_HIGH;
        BIN2_LOW;
    }
    else
    {
        /* 刹车 AIN=1 AIN2=1 */
        BIN1_HIGH;
        BIN2_HIGH;
    }
}

void Int_TB6612_SetPWM(int pwma, int pwmb)
{
    if (pwma > 0)
    {
        /* 正转 */
        Int_TB6612_MotorA(MOTOR_FORWARD);
    }
    else if (pwma < 0)
    {
        /* 反转 */
        Int_TB6612_MotorA(MOTOR_BACK);
    }
    else
    {
        /* 刹车 */
        Int_TB6612_MotorA(MOTOR_BRAKE);
    }

    if (pwmb > 0)
    {
        /* 正转 */
        Int_TB6612_MotorB(MOTOR_FORWARD);
    }
    else if (pwmb < 0)
    {
        /* 反转 */
        Int_TB6612_MotorB(MOTOR_BACK);
    }
    else
    {
        /* 刹车 */
        Int_TB6612_MotorB(MOTOR_BRAKE);
    }

    /* 设置占空比（车速） */
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, abs(pwma));
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, abs(pwmb));

}
