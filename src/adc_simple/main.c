#include "pins.h"
#include "delay.h"
#include "uart.h"

int main(void) {
  usart1_setup_default();
  usart1_putline("\r\nadc_simple");

  // Enable ADC1 clock
  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

  // Turn ADC1, wait a bit and start a calibration run
  ADC1->CR2 |= ADC_CR2_ADON;
  delay_us(1);
  ADC1->CR2 |= ADC_CR2_CAL;

  // Wait until calibration is complete
  while (ADC1->CR2 & ADC_CR2_CAL);
  uint32_t cal = ADC1->DR;

  usart1_printf("Calibration complete: %d", cal);

  pin_set_input_analog(A, 0);
  ADC1->SQR1 |= 0 << ADC_SQR1_L_Pos; // Sample 1 channel only
  ADC1->SQR3 |= 0; // Sample channel 0 on PA0 first

  while (1) {
    usart1_putline("Starting conversion");
    
    // Start single conversion
    ADC1->CR2 |= ADC_CR2_ADON;

    // Wait for end of conversion flag
    while (!(ADC1->SR & ADC_SR_EOC));
    
    // Read ADC_DR
    uint32_t result = ADC1->DR;

    usart1_printf("Read: %d, %f V", result, 3.3d * (double)result / 4096.0d);

    delay_ms(1000);
  }
}

