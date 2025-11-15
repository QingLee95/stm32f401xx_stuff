#include "uart.h"
#include "debug_led.h"
#include "gpio.h"

#include <stm32f4xx.h>
#include <stdio.h>

#define BAUDRATE 115200
#define UART USART2
#define MSG_SIZE 3
static const char* msg[MSG_SIZE] = {"Exercise:", "	UART", "	GPIO: input"};

//define when printf is used it is written to the uart
int __io_putchar(int ch){
	if(uart_write(UART, (uint8_t)(ch & 0xFF)) == OK)
	{
		return ch;
	}
	return -1;
}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
	led_init();
	led_off();
	if(uart_init(GPIOA, UART, BAUDRATE) != OK)
	{
		return -1;
	}

	gpio_init_user_btn();
	led_on();
	uint8_t index = 0;
	while(1){
		if(gpio_user_btn_pressed()){
			printf("\r%s\n", msg[index++%MSG_SIZE]);
		}
	}
	return 0;
}

