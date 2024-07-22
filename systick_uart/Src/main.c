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
#define TIME_FORMAT_SIZE 9
#define INDEX_HOUR 0
#define INDEX_MIN 3
#define INDEX_SEC 6

//"00:00:00\0"
static char s_time_format[TIME_FORMAT_SIZE]="00:00:00\0";


//define when printf is used it is written to the uart
int __io_putchar(int ch){
	if(uart_write(UART, (uint8_t)(ch & 0xFF)) == OK)
	{
		return ch;
	}
	return -1;
}


void convert_number_two_digits(char* str, uint8_t digits, uint8_t index){
	if(digits < 10){
		str[index]= '0';
		str[index+1]= ('0' + digits);
	}
	else
	{
		str[index] = ('0' + (digits/10));
		str[index+1]= ('0' + (digits%10));
	}

}

void format_time(char* str, uint8_t hours, uint8_t mins, uint8_t secs){
	if(strlen(str) < (TIME_FORMAT_SIZE - 1)){
		return;
	}
	convert_number_two_digits(str, hours, INDEX_HOUR);
	convert_number_two_digits(str, mins, INDEX_MIN);
	convert_number_two_digits(str, secs, INDEX_SEC);
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
		uint32_t mins = seconds/60;
		uint32_t hours = mins/60;
		format_time(s_time_format, (uint8_t)(hours%100), (uint8_t)(mins%60), (uint8_t)(seconds%60));
		printf("\r%s", s_time_format);

		//printf prints to stdout standard and that is buffered. Without newline it will print nothing
		//io_putchar() will not be called. Calling fflush(stdout) will flush the buffer
		fflush(stdout);
		systick_delay_ms(1000);
		++seconds;
	}
	return 0;
}

