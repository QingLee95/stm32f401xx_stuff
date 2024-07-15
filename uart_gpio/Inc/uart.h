#pragma once

#include <stm32f4xx.h>
#include <stdint.h>


typedef enum ERR_CODE{
	OK = 0,
	INCORRECT_GPIO_TX =-1,
	NOT_INITIALIZED = -2,
	ALREADY_INITIALIZED = -3,
	UNSUPPORTED = -4,
} ErrCode;

/*
 * return 0 OK
 * */
ErrCode uart_init(GPIO_TypeDef* gpio_tx, USART_TypeDef* uart, uint32_t baudrate);

ErrCode uart_write(USART_TypeDef* uart, uint8_t ch);
