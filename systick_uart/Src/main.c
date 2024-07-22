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
#include <string.h>

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

// str needs to contain 3 elements (2 digits and null char)
void convertNumberTwoDigits(char* str, uint8_t digits){
	if(strlen(str) != 2 || digits > 100){
		return;
	}
	if(digits < 10){
		str[0]= '0';
		//48 == '0' ascii
		str[1]= ('0' + digits);
	}
	else
	{
		//48 == '0' ascii
		str[0] = ('0' + (digits/10));
		str[1]= ('0' + (digits-10));
	}

}

/**
  * @brief  The application entry point.
  * @retval int
  * Only support hour up until 99
  */
int main(void)
{
	if(uart_init(UART, BAUDRATE) != OK)
	{
		return -1;
	}

	//clear screen
	printf("\r                                      ");
	uint32_t seconds = 0;
	while(1){
		uint8_t mins = seconds/60;
		uint8_t hours = mins/60;
		char secsStr[] = "00";
		char minsStr[] = "00";
		char hoursStr[] = "00";
		convertNumberTwoDigits(secsStr, (uint8_t)(seconds%60));
		convertNumberTwoDigits(minsStr, mins);
		convertNumberTwoDigits(hoursStr, hours);
		printf("\r%s:%s:%s\r", hoursStr, minsStr, secsStr);

		//printf prints to stdout standard and that is buffered. Without newline it will print nothing
		//io_putchar() will not be called. Calling fflush(stdout) will flush the buffer
		fflush(stdout);
		systick_delay_ms(1000);
		++seconds;

	}
	return 0;
}

