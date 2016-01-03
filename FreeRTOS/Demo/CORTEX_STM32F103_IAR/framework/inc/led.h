#ifndef _LED_H_
#define _LED_H_


#define LED0_RCC	RCC_APB2Periph_GPIOF 
#define LED1_RCC	RCC_APB2Periph_GPIOF 

#define LED0_PORT	GPIOF
#define LED1_PORT	GPIOF

#define LED0_PIN	GPIO_Pin_6
#define LED1_PIN	GPIO_Pin_7

void led_init(void);

#endif	/* _LED_H_ */
