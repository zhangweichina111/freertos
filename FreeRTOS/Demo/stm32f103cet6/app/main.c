#include "led.h"
#include "usart.h"
#include "printf.h"
#include "stm32f10x_conf.h"
#include "FreeRTOS.h"
#include "task.h"

#define LED_TASK_STACK_SIZE		(configMINIMAL_STACK_SIZE + 16)
#define LED_TASK_PRIORITY		(tskIDLE_PRIORITY + 1)

#define USART1_TASK_STACK_SIZE	(configMINIMAL_STACK_SIZE + 16)
#define USART1_TASK_PRIORITY	(tskIDLE_PRIORITY + 2)

static void led_task(void *args)
{
	TickType_t xPreviousWakeTime;
	xPreviousWakeTime = xTaskGetTickCount();
	for (;;) {
		led_on(LED0_PORT, LED0_PIN);
		led_off(LED1_PORT, LED1_PIN);
		vTaskDelayUntil(&xPreviousWakeTime, 1000 / portTICK_PERIOD_MS);
		led_off(LED0_PORT, LED0_PIN);
		led_on(LED1_PORT, LED1_PIN);
		vTaskDelayUntil(&xPreviousWakeTime, 1000 / portTICK_PERIOD_MS);
	}
}

static void usart1_task(void *args)
{
	for (;;) {
		usart_send_string(USART1, (const char *)args);
                usart_printf("hello_printf %d\n",5);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}

static void prvSetupHardware( void )
{
	/* Start with the clocks in their expected state. */
	RCC_DeInit();

	/* Enable HSE (high speed external clock). */
	RCC_HSEConfig( RCC_HSE_ON );

	/* Wait till HSE is ready. */
	while( RCC_GetFlagStatus( RCC_FLAG_HSERDY ) == RESET )
	{
	}

	/* 2 wait states required on the flash. */
	*( ( unsigned long * ) 0x40022000 ) = 0x02;

	/* HCLK = SYSCLK */
	RCC_HCLKConfig( RCC_SYSCLK_Div1 );

	/* PCLK2 = HCLK */
	RCC_PCLK2Config( RCC_HCLK_Div1 );

	/* PCLK1 = HCLK/2 */
	RCC_PCLK1Config( RCC_HCLK_Div2 );

	/* PLLCLK = 8MHz * 9 = 72 MHz. */
	RCC_PLLConfig( RCC_PLLSource_HSE_Div1, RCC_PLLMul_9 );

	/* Enable PLL. */
	RCC_PLLCmd( ENABLE );

	/* Wait till PLL is ready. */
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
	{
	}

	/* Select PLL as system clock source. */
	RCC_SYSCLKConfig( RCC_SYSCLKSource_PLLCLK );

	/* Wait till PLL is used as system clock source. */
	while( RCC_GetSYSCLKSource() != 0x08 )
	{
	}

	/* Enable GPIOA, GPIOB, GPIOC, GPIOD, GPIOE,GPIOF GPIOG and AFIO clocks */
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC
							| RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO
							RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG, ENABLE );

	/* SPI2 Periph clock enable */
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI2, ENABLE );


	/* Set the Vector Table base address at 0x08000000 */
	NVIC_SetVectorTable( NVIC_VectTab_FLASH, 0x0 );

	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );

	/* Configure HCLK clock as SysTick clock source. */
	SysTick_CLKSourceConfig( SysTick_CLKSource_HCLK );


}

int main()
{
    prvSetupHardware();
	led_init();
	usart1_init(115200);

	xTaskCreate(led_task, "led_task", LED_TASK_STACK_SIZE, NULL, LED_TASK_PRIORITY, NULL);
	xTaskCreate(usart1_task, "usart1_task", USART1_TASK_STACK_SIZE, "Hello World\r\n", USART1_TASK_PRIORITY, NULL);

	vTaskStartScheduler();

	return 0;
}
