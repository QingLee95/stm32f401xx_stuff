#include "debug_led.h"
#include <stm32f4xx.h>

#define GPIOAEN 		(1U<<0)
#define GPIOA_MODER_5	(1UL<<10)
#define PIN5            (1UL<<5)

void led_init(){
	RCC->AHB1ENR |= GPIOAEN;
	GPIOA->MODER |= GPIOA_MODER_5;
}

void led_on(){
	GPIOA->ODR |= PIN5;
}

void led_off(){
	GPIOA->ODR &= ~PIN5;
}
