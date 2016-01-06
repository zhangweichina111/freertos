#include "led.h"

void led_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(LED0_RCC | LED1_RCC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin   = LED0_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(LED0_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = LED1_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(LED1_PORT, &GPIO_InitStructure);
}

void led_on(GPIO_TypeDef* port, uint16_t pin)
{
	// port->BSRR = pin;
	GPIO_SetBits(port,pin);
}

void led_off(GPIO_TypeDef* port, uint16_t pin)
{
	// port->BRR = pin;
	GPIO_SetBits(port,pin);
}
