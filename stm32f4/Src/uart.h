#ifndef UART_H
#define UART_H
#include "stm32f4xx.h"

enum
{Forward = 1, Back, Right, Left, Up, Down, CW, CCW, Error = 255};

void USART1_Send(uint8_t byte);
void USART1_Send_String(uint8_t* out, uint8_t size);
void USART1_IRQHandler();
void USART1_Init();
#endif