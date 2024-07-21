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
#include "systick.h"
#include <stm32f4xx.h>
#include <stdio.h>

#define BAUDRATE 115200
#define UART USART2


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
	if(uart_init(UART, BAUDRATE) != OK)
	{
		return -1;
	}

	uint32_t seconds = 0;
	while(1){
		uint8_t mins = seconds/60;
		uint8_t hours = mins/60;
		printf("\r%hu:%hu:%hu\n", hours, (mins%60), (seconds%60));
        systick_delay_ms(1000);
        ++seconds;

	}
	return 0;
}

