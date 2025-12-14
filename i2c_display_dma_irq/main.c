#include "i2c.h"

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
	I2C_Config config;
	config.mode = I2C_FAST;
	config.scl_pin = I2C1_PB8;
	config.sda_pin = I2C1_PB9;

	i2c_init(&config);

	return 0;
}

