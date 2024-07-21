#include "systick.h"
#include <stm32f4xx.h>

#define DEFAULT_CLK		16000000U
//When value is 0 1 ms will be passed
#define CLK_COUNTER_MS (DEFAULT_CLK/1000)
#define COUNT_FLAG (1U << 16)
#define ENABLE (1U)
#define CLKSOURCE (1U << 2)

void systick_delay_ms(uint32_t delay){
	SysTick->LOAD = CLK_COUNTER_MS;
	SysTick->CTRL |= (ENABLE | CLKSOURCE);

	for(uint32_t i = 0; i<delay; i++){
		//Wait 1 ms
		while(!(SysTick->CTRL & COUNT_FLAG)){};
	}
	SysTick->CTRL &= ~ENABLE;
}
