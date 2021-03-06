#ifndef USART_H
#define USART_H
#include "stm32.h"
#include "GPIO.h"
#include "u_string.h"

void USART_init(void* buffer, size_t buf_size);
int write_DMA_USART(const char* buffer, uint16_t buf_size);
void puts(const char* buffer);
// void _write_DMA_USART(const char* buffer, uint16_t buf_size);
size_t USART_DMA_get_pos(void);

void USART_sendChr(char Ch);
void USART_sendString(char* STR);
int USART_sendStringi(char* STR,int i);


#endif // !USART_H
