#include "Int_Encoder.h"
#include "tim.h"


void Int_Encoder_Init(void)
{
    /* 开启编码器 */
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_1 | TIM_CHANNEL_2);
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_1 | TIM_CHANNEL_2);
}


int16_t Int_Encoder_ReadCounter(uint8_t motor)
{
    int16_t  encoder_val;
    switch (motor)
    {
    case MOTOR_A:
    {
        /* 读取定时器 */
        encoder_val = -1 * ((int16_t)__HAL_TIM_GET_COUNTER(&htim2));
        /* 清空定时器 */
        __HAL_TIM_SET_COUNTER(&htim2, 0);
        break;
    }
    case MOTOR_B:
    {
        encoder_val = (int16_t)__HAL_TIM_GET_COUNTER(&htim3);
        /* 清空定时器 */
        __HAL_TIM_SET_COUNTER(&htim3, 0);
        break;
    }
    default:
        break;
    }
    /* 返回带符号的计数值 */
    return encoder_val;
}
