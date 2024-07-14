#include "uart.h"

// Definitions
/*
 * UASART1 TX = PA9
 * USART2 TX = PD5
 */
#define GPIOAEN 		(1U<<0)
#define GPIODEN 		(1U<<3)
#define MODER9_BIT      18
#define MODER5_BIT		10
#define AFRL5_BIT       20
#define AFRH9_BIT		4
#define AF7				0b0111
#define CR1_UA			(1U<<13)
#define CR1_TE			(1U<<3)
#define OVER8 			0U
#define DEFAULT_CLK		16000000U
#define SR_TC			(1u<<6)

static uint8_t s_initialized = 0;

// Help functions
static inline uint8_t is_gpio_a(GPIO_TypeDef* gpio){
	return gpio == GPIOA;
}

static inline uint8_t is_gpio_d(GPIO_TypeDef* gpio){
	return gpio == GPIOD;
}

static ErrCode configure_uart_tx_pin(GPIO_TypeDef* gpio_tx){
	if(!is_gpio_a(gpio_tx) && !is_gpio_d(gpio_tx)){
		return INCORRECT_GPIO_TX;
	}
		//Enable clock for GPIO
	RCC->AHB1ENR |= (is_gpio_a(gpio_tx)? GPIOAEN : GPIODEN);
	//Set mode of GPIO to alternate function (10)
	uint8_t bit_shift = is_gpio_a(gpio_tx) ? MODER9_BIT : MODER5_BIT;
	//Set to 0
	gpio_tx->MODER &=~(1<<bit_shift);
	//Set to 1
	gpio_tx->MODER |=(1<<(bit_shift + 1));
	//Set Alternate function register to AF7 (USART)
	bit_shift = is_gpio_a(gpio_tx) ? AFRH9_BIT : AFRL5_BIT;
	//Low register for port 0-7 and High register for port 8-15
	volatile uint32_t* afr = (is_gpio_a(gpio_tx) ? &gpio_tx->AFR[1] : &gpio_tx->AFR[0]);
	*afr |= (AF7 << bit_shift);
	*afr &= ~(1 << (bit_shift + 3));
	return OK;
}

static uint16_t calc_usartdiv(uint32_t baudrate){
	uint16_t ret = 0;
	uint8_t sampling = 8 *(2 - OVER8);
	//float to integer discard after comma
	uint16_t mantissa = DEFAULT_CLK/(baudrate * sampling);
	//Contains 2 number after comma x100
	uint32_t realNox100 = (DEFAULT_CLK*100)/(baudrate * sampling);
	//50 is to add rounding + multiply by 8 * (2- OVER8)
	uint16_t fraction = (((realNox100 - (mantissa * 100))* sampling) + 50)/100;
	ret |= mantissa << 4;
	ret |= fraction;
	return ret;
}


// Public functions
ErrCode uart_init(GPIO_TypeDef* gpio_tx, USART_TypeDef* uart, uint32_t baudrate){
	if(s_initialized){
		return ALREADY_INITIALIZED;
	}

	ErrCode ret = OK;
	if((ret = configure_uart_tx_pin(gpio_tx)) != OK){
		return ret;
	}
	/* Reset to default values:
	 * 1 stop bit
	 * 8 Data bits
	 * No DMA
	 * no interrupt
	 */
    uart->CR1 = 0;
    uart->CR2 = 0;
    uart->CR3 = 0;
    //Enable uart
    uart->CR1 |= CR1_UA;
    //Set baudrate
    uart->BRR |= calc_usartdiv(baudrate);
    //Enable transmitter
    uart->CR1 |= CR1_TE;
    s_initialized = 1;
	return ret;
}

ErrCode uart_write(USART_TypeDef* uart, uint8_t ch){
	if(!s_initialized){
		return NOT_INITIALIZED;
	}
	//Wait until byte is transfered
	while((uart->SR & SR_TC) == 0){}

	uart->DR = (uint32_t) ch;
	return OK;
}
