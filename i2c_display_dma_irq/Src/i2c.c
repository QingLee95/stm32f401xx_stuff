#include "i2c.h"

#include <stddef.h>

#define SUCCESS 0
#define ERROR  -1

#define NR_OF_PINS 2
#define AF_REG_MAX 8

/********************** REGISTER VALUES **************************************/
// GPIO
#define RCC_GPIOAEN         (1<<0)
#define RCC_GPIOBEN         (1<<1)
#define RCC_GPIOCEN         (1<<2)

// I2C
#define RCC_I2C1EN          (1<<21)
#define RCC_I2C2EN          (1<<22)
#define RCC_I2C3EN          (1<<23)


// To support fast mode you need a multiple of 10MHz
// Limited by APB1 max bus freq which is 42MHz
#define I2C_CLK_FRQ         (30)  //MHz 
#define I2C_CCR_100kHz      (int)(I2C_CLK_FRQ / (2 * 0.1f))
#define I2C_CCR_400kHz      (int)(I2C_CLK_FRQ / (25 * 0.4f))
#define I2C_TRISE_100kHz    (int)(I2C_CLK_FRQ + 1)
#define I2C_TRISE_400kHz    (int)((I2C_CLK_FRQ * 0.3f) + 1)

/********************** STATIC FUNCTIONS **************************************/
static int enable_clock(const I2C_Config* config);
static int configure_gpio(const I2C_Config* config);
static int configure_i2c(const I2C_Config* config);

static I2C_TypeDef *_i2c_device = NULL;

/********************** HEADER FUNCTIONS **************************************/
int i2c_init(const I2C_Config* config)
{
    if(!config){
        return ERROR;
    }
    
    if(enable_clock(config) != SUCCESS){
        return ERROR;
    }
    if(configure_gpio(config) != SUCCESS){
        return ERROR;
    }
    if(configure_i2c(config) != SUCCESS){
        return ERROR;
    }

    _i2c_device = get_i2c(config->scl_pin, config->sda_pin);
    return SUCCESS;
}

/********************** STATIC FUNCTIONS **************************************/
static int enable_clock(const I2C_Config* config){
    GPIO_TypeDef* gpio = get_scl_gpio(config->scl_pin);
    if(gpio == GPIOB){
        RCC->AHB1ENR |= RCC_GPIOBEN;
    } else if(gpio == GPIOA){
        RCC->AHB1ENR |= RCC_GPIOAEN;
    } else{
        return ERROR;  
    }

    gpio = get_sda_gpio(config->sda_pin);
    if(gpio == GPIOB){
        RCC->AHB1ENR |= RCC_GPIOBEN;
    } else if(gpio == GPIOC){
        RCC->AHB1ENR |= RCC_GPIOAEN;
    } else{
        return ERROR;  
    }

    I2C_TypeDef* i2c = get_i2c(config->scl_pin, config->sda_pin);
    if(i2c == I2C1){
        RCC->APB1ENR |= RCC_I2C1EN;
    } else if(i2c == I2C2){
        RCC->APB1ENR |= RCC_I2C2EN;
    }  else if(i2c == I2C3){
        RCC->APB1ENR |= RCC_I2C3EN;
    } else{
        return ERROR;  
    }
    return SUCCESS;
}

static int configure_gpio(const I2C_Config* config){
    int8_t pin_nrs[NR_OF_PINS] = {scl_to_pin_nr(config->scl_pin), sda_to_pin_nr(config->sda_pin)};
    GPIO_TypeDef* gpios[NR_OF_PINS] = {get_scl_gpio(config->scl_pin), get_sda_gpio(config->sda_pin)};
    int8_t af_values[NR_OF_PINS] = {scl_to_af(config->scl_pin), sda_to_af(config->sda_pin)};

    for(uint8_t i = 0; i < NR_OF_PINS; i++){
        
        int8_t pin_nr = pin_nrs[i];
        GPIO_TypeDef* gpio = gpios[i];
        int8_t af_value = af_values[i];
        uint8_t af_index = pin_nr >= AF_REG_MAX ? 1 : 0;
        
        if(pin_nr < 0 || !gpio || af_value < 0){
            return ERROR;
        }

        // Configure as Alternate Function
        gpio->MODER &= ~(1 << (pin_nr * 2));
        gpio->MODER |=  (1 << ((pin_nr * 2) + 1));

        // Configure as output type open drain
        gpio->OTYPER |= (1 << pin_nr);

        // Enable pull up
        gpio->PUPDR |= (1 << (pin_nr * 2));
        gpio->PUPDR &= ~(1 << ((pin_nr * 2) + 1));

        // AF register
        // mod 8 because the AFR[1] start at 0 for pin 8
        gpio->AFR[af_index] &= ~(0x0F << ((pin_nr % AF_REG_MAX) * 4));
        //The 4 bits of the register represent the number in AFx
        gpio->AFR[af_index] |= (af_value << ((pin_nr % AF_REG_MAX) * 4));
    }
    return SUCCESS;
}

static int configure_i2c(const I2C_Config* config){
    I2C_TypeDef* i2c = get_i2c(config->scl_pin, config->sda_pin);
    if(!i2c){
        return ERROR;
    }

    // Reset
    i2c->CR1 |= I2C_CR1_SWRST;
    i2c->CR1 &= ~I2C_CR1_SWRST;
    
    // Clock frequentie
    i2c->CR2 |= I2C_CLK_FRQ;

    // Default standard mode
    i2c->CCR &= ~I2C_CCR_FS;
    i2c->CCR &= ~I2C_CCR_DUTY;
    i2c->CCR |= (I2C_CCR_100kHz & 0x0FFF);
    i2c->TRISE |= (I2C_TRISE_100kHz & 0x3F);
 
    if(config->mode == I2C_FAST){
        i2c->CCR |= I2C_CCR_FS;
        i2c->CCR |= I2C_CCR_DUTY;
        i2c->CCR |= (I2C_CCR_400kHz & 0x0FFF);
        i2c->TRISE |= (I2C_TRISE_400kHz & 0x3F);
    }

    //Enable i2c device
    i2c->CR1 |= I2C_CR1_PE;
    return SUCCESS;
}
