#include "Int_HC05.h"
#include <stdio.h>
#include "App_Car.h"
uint8_t hc05_recv;


void Int_HC05_Init(void)
{
    HAL_UART_Receive_IT(&huart2, &hc05_recv, 1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{
    if (huart->Instance == huart2.Instance)
    {
        switch ((char)hc05_recv)
        {
        case 'U':
            flag_up = 1;
            flag_down = flag_left = flag_right = 0;
            break;
        case 'D':
            flag_down = 1;
            flag_up = flag_left = flag_right = 0;
            break;
        case 'L':
            flag_left = 1;
            flag_up = flag_right = flag_down = 0;
            break;
        case 'R':
            flag_right = 1;
            flag_up = flag_left = flag_down = 0;
            break;
        default:
            flag_up = flag_down = flag_left = flag_right = 0;
            break;
        }
        HAL_UART_Receive_IT(&huart2, &hc05_recv, 1);
    }
}
