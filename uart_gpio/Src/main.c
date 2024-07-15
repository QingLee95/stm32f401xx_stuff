/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "uart.h"
#include "debug_led.h"
#include <stm32f4xx.h>

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
	led_init();
	led_off();
	if(uart_init(GPIOA, USART2, 115200) == OK)
	{
		led_on();
		while(1){
			if(uart_write(USART2,(uint8_t)'h') != OK){
				led_off();
			}
		}
	}

	return 0;
}

