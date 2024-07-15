#include "gpio.h"
#include <stm32f4xx.h>

// User button = PC13
#define GPIOCEN 	(1U<<2)
#define MODER13_BIT 26
#define IDR13		(1U<<13)
#define PUPDR13_BIT 26

static uint8_t s_initialized_user_btn = 0;
static uint8_t s_user_btn_pressed = 0;

void gpio_init_user_btn(){
	if(s_initialized_user_btn)
		return;
	//Enable clock for GPIOC
	RCC->AHB1ENR |= GPIOCEN;
	//Configure as input
	GPIOC->MODER &= ~(3 << MODER13_BIT);
	//PULL-UP = pressed = 0 and not pressed 1
	GPIOC->PUPDR |= 1 << PUPDR13_BIT;
	GPIOC->PUPDR &= ~(1 << (PUPDR13_BIT +1));
	s_initialized_user_btn = 1;
	s_user_btn_pressed = (uint8_t) ((GPIOC->IDR & IDR13) == 0);
}


uint8_t gpio_user_btn_pressed(){
	if(!s_initialized_user_btn){
		return 0;
	}
	uint8_t btn_is_pressed = (uint8_t) ((GPIOC->IDR & IDR13) == 0);
	//If it was already pressed return not pressed
	if(s_user_btn_pressed){
		//Pressed and then not pressed
		s_user_btn_pressed = btn_is_pressed;
		return 0;
	}

	//Not pressed see if it is pressed
	s_user_btn_pressed = btn_is_pressed;
	return s_user_btn_pressed;
}
