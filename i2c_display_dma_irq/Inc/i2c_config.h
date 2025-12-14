#pragma once

#include <stm32f4xx.h>
#include <stdint.h>

typedef enum {
    I2C_STANDARD,
    I2C_FAST
} I2C_Mode;

typedef enum {
    I2C1_PB6,
    I2C1_PB8,
    I2C2_PB10,
    I2C3_PA8
} I2C_SCL;

typedef enum {
    I2C1_PB7,
    I2C1_PB9,
    I2C2_PB3,
    I2C3_PB4,
    I2C3_PC9
} I2C_SDA;


typedef struct{
    I2C_Mode mode;
    I2C_SCL scl_pin;
    I2C_SDA sda_pin;
} I2C_Config;

// return negative number if not supported
int8_t scl_to_pin_nr(I2C_SCL scl);
int8_t sda_to_pin_nr(I2C_SDA sda);
// AF{return value} negative -> not supported
int8_t scl_to_af(I2C_SCL scl);
int8_t sda_to_af(I2C_SDA sda);

// Return NULL if not supported
GPIO_TypeDef* get_scl_gpio(I2C_SCL scl);
GPIO_TypeDef* get_sda_gpio(I2C_SDA sda);
I2C_TypeDef* get_i2c(I2C_SCL scl, I2C_SDA sda);