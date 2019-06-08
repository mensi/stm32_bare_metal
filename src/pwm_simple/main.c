#include "pins.h"
#include "uart.h"
#include "config.h"

int main(void) {
  usart1_setup_default();
  usart1_putline("\r\npwm_simple on B9");

  // Let's use B9 - the default alternate function on this pin is TIM4_CH4
  pin_set_output_50mhz_alt(B, 9);

  // Enable clock for TIM4
  RCC->APB1ENR |= RCC_APB1ENR_TIM4EN; // Enable clock to TIM

  TIM4->PSC = F_CPU / 1000000; // Set up prescaler so we end up with a 1 MHz counter clock
  TIM4->ARR =  10000; // Auto reload value is 10000 which is reached after 10ms. This is the PWM period.
  TIM4->CCR4 =  1000; // Compare value for channel 4. When this is crossed, the output will change.
  TIM4->CCMR2 |= (TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1); // Set channel 4 mode to PWM 1 - high first, then low
  TIM4->CCER |= TIM_CCER_CC4E; // Enable channel 4
  TIM4->CR1 |= TIM_CR1_CEN; // Enable the timer
}

