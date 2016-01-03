#ifndef _USART_H_
#define _USART_H_

#include <stdint.h>
#include "stm32f10x_usart.h"

void usart1_init(uint32_t baudrate);
void usart_send_byte(USART_TypeDef* usartx, uint16_t data);
void usart_send_halfword(USART_TypeDef* usartx, uint16_t data);
void usart_send_word(USART_TypeDef* usartx, uint32_t data);
void usart_send_string(USART_TypeDef* usartx, const char* data);

#endif	/* _USART_H_ */
