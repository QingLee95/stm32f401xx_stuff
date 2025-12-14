#pragma once

#include "i2c_config.h"

/**
 * @brief Initialize the i2c device
 *        Needs to be called before other i2c functions can be used
 * @param config 
 * @return int 0 = success 
 */
int i2c_init(const I2C_Config* config);

void i2c_start(void);
int i2c_send_address(uint8_t address_slave);
int i2c_send_data(uint8_t byte);
void i2c_stop(void);
