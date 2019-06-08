#include "pins.h"
#include "uart.h"
#include "delay.h"

int main(void) {
  usart1_setup_default();
  usart1_putline("\r\nbitbang_square_wave");

  pin_set_output_50mhz(B, 12);

  while(1) {
    pin_gpio_set_high(B, 12);
    delay_us(10);
    pin_gpio_set_low(B, 12);
    delay_us(15);
  }
}
