#include "uart.h"

// Definitions
/*
 * USART1 TX = PA9
 * USART2 TX = PA2
 */
#define GPIOAEN 		(1U<<0)
#define MODER9_BIT      18
#define MODER2_BIT		4
#define AFRL2_BIT       8
#define AFRH9_BIT		4
#define AF7				0b0111
#define CR1_UE			(1U<<13)
#define CR1_TE			(1U<<3)
#define OVER8 			0U
#define DEFAULT_CLK		16000000U
#define SR_TXE			(1u<<7)
#define USART2EN		(1u<<17)

static uint8_t s_initialized = 0;

// Help functions
static inline uint8_t is_gpio_a(GPIO_TypeDef* gpio){
	return gpio == GPIOA;
}

static inline uint8_t is_usart_1(USART_TypeDef* uart){
	return uart == USART1;
}

static inline uint8_t is_usart_2(USART_TypeDef* uart){
	return uart == USART2;
}

static void configure_uart_tx_pin(USART_TypeDef* uart){
	//TODO: Check for other uarts
	GPIO_TypeDef* gpio_tx = GPIOA;
	//Enable clock for GPIO
	RCC->AHB1ENR |=  GPIOAEN;
	//Set mode of GPIO to alternate function (10)
	uint8_t bit_shift = is_usart_1(uart) ? MODER9_BIT : MODER2_BIT;
	//Set to 0
	gpio_tx->MODER &=~(1<<bit_shift);
	//Set to 1
	gpio_tx->MODER |=(1<<(bit_shift + 1));
	//Set Alternate function register to AF7 (USART)
	bit_shift = is_usart_1(uart) ? AFRH9_BIT : AFRL2_BIT;
	//Low register for port 0-7 and High register for port 8-15
	volatile uint32_t* afr = (is_usart_1(uart) ? &gpio_tx->AFR[1] : &gpio_tx->AFR[0]);
	*afr |= (AF7 << bit_shift);
	*afr &= ~(1 << (bit_shift + 3));
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

static ErrCode validate(USART_TypeDef* uart){
	if(s_initialized){
		return ALREADY_INITIALIZED;
	}
	//At the moment only usart2 supported TODO:Others
	if(!is_usart_2(uart)){
		return UNSUPPORTED;
	}
	return OK;
}


// Public functions
ErrCode uart_init(USART_TypeDef* uart, uint32_t baudrate){
	ErrCode ret = OK;
	if((ret = validate(uart)) != OK){
		return ret;
	}

	configure_uart_tx_pin(uart);
	//Enable clock for uart
	if(is_usart_2(uart)){
		RCC->APB1ENR |= USART2EN;
	}
    //TODO: other uarts
    //Set baudrate
    uart->BRR = calc_usartdiv(baudrate);
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
    uart->CR1 |= CR1_UE;
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
	while(!(uart->SR & SR_TXE)){}

	uart->DR = ch;
	return OK;
}
