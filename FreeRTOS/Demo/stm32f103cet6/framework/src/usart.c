#include "usart.h"
#include "misc.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

/* UART interrupt handler. */
void vUARTInterruptHandler( void );

void usart1_init(uint32_t baudrate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO, ENABLE);
	
	/*
	 * USART1_TX -> PA9, USART1_RX -> PA10
	 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* Configure the NVIC Preemption Priority Bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	/* Enable the USART1 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);
}

void usart_send_byte(USART_TypeDef* usartx, uint16_t data)
{
	while (!(usartx->SR & USART_FLAG_TXE));
	usartx->DR = (data & (uint16_t)0x01FF);
}

void usart_send_halfword(USART_TypeDef* usartx, uint16_t data)
{
	usart_send_byte(usartx, data);
	usart_send_byte(usartx, data >> 8);
}

void usart_send_word(USART_TypeDef* usartx, uint32_t data)
{
	usart_send_byte(usartx, data);
	usart_send_byte(usartx, data >> 8);
	usart_send_byte(usartx, data >> 16);
	usart_send_byte(usartx, data >> 24);
}

void usart_send_string(USART_TypeDef* usartx, const char* data)
{
	while (*data) {
		usart_send_byte(usartx, *data);
		data++;
	}
}


void vUARTInterruptHandler(void)
{
	if (USART_GetFlagStatus(USART1, USART_IT_RXNE) != RESET) {
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		usart_send_byte(USART1, USART1->DR);
	}
#if 0
	static uint8_t syn, index;
	unsigned char data;
	
	if (USART_GetFlagStatus(USART1, USART_IT_RXNE) != RESET) {
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		data = USART1->DR;
		 //usart_send_byte(USART1, data); 
		if (data == 0xAA && syn == 0) {
			syn = 1;
			index = 0;
		}
		if (syn == 1) {
			rev_buffer[index] = data;
		}
		if (index == rev_buffer[1] - 1) {
			syn = 0;
			usart1_received = 1;
		}
		index++;
	}
#endif
}
