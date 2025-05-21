#ifndef __INT_HC05_H__
#define __INT_HC05_H__

#include "usart.h"

extern uint8_t hc05_recv;

/**
 *@brief 初始化蓝牙接收
 *
 */
void Int_HC05_Init(void);

#endif /* __INT_HC05_H__ */
