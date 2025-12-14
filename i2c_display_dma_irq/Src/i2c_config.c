#include "i2c_config.h"

#include <stddef.h>

int8_t scl_to_pin_nr(I2C_SCL scl){
    switch (scl)
    {
    case I2C1_PB6:
        return 6;
    case I2C1_PB8:
    case I2C3_PA8:
        return 8;
    case I2C2_PB10:
        return 10;
    default:
        return -1;
    }
}

int8_t sda_to_pin_nr(I2C_SDA sda){
    switch (sda)
    {
    case I2C2_PB3:
        return 3;
    case I2C3_PB4:
        return 4;
    case I2C1_PB7:
        return 7;
    case I2C1_PB9:
    case I2C3_PC9:
        return 9;
    default:
        return -1;
    }
}

int8_t scl_to_af(I2C_SCL scl){
    switch (scl)
    {
    case I2C1_PB6:
    case I2C1_PB8:
    case I2C2_PB10:
    case I2C3_PA8:
        return 4;
    default:
        return -1;
    }
}

int8_t sda_to_af(I2C_SDA sda){
    switch (sda)
    {
    case I2C1_PB7:
    case I2C1_PB9:
    case I2C3_PC9:
        return 4;
    case I2C2_PB3:
    case I2C3_PB4:
        return 9;
    default:
        return -1;
    }
}

GPIO_TypeDef* get_scl_gpio(I2C_SCL scl){
    switch (scl)
    {
    case I2C1_PB6:
    case I2C1_PB8:
    case I2C2_PB10:
        return GPIOB;
    case I2C3_PA8:
        return GPIOA;
    default:
        return NULL;
    }
}

GPIO_TypeDef* get_sda_gpio(I2C_SDA sda){
    switch (sda)
    {
    case I2C2_PB3:
    case I2C3_PB4:
    case I2C1_PB7:
    case I2C1_PB9:
        return GPIOB;
    case I2C3_PC9:
        return GPIOC;
    default:
        return NULL;
    }
}

I2C_TypeDef* get_i2c(I2C_SCL scl, I2C_SDA sda){
    if((scl == I2C1_PB6 || scl == I2C1_PB8) && (sda == I2C1_PB7 || sda == I2C1_PB9)){
        return I2C1;
    }
    if(scl == I2C2_PB10 && sda == I2C2_PB3){
        return I2C2;
    }
    if(scl == I2C3_PA8 && (sda == I2C3_PB4 || sda == I2C3_PC9)){
        return I2C3;
    }
    return NULL;
}