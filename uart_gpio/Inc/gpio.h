#pragma once

#include <stdint.h>


void gpio_init_user_btn();

/*
 * Function needs to be called in a loop
 * return 1 when it goes from not pressed to pressed
 * */
uint8_t gpio_user_btn_pressed();
