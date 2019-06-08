#include "pins.h"
#include "interrupt.h"
#include "uart.h"
#include "config.h"

void delay(unsigned int duration) {
  // Hacky approximation: We're running at 72 MHz by default, so let's
  // just take something around 10 cycles long and do it 72000/10 times
  // to wait one millisecond. With interrupts disabled, this should be
  // in the ballpark of real time.
  while (duration > 0) {
    int inner = 72000 / 10;
    while (inner--) {
      __asm__ volatile ("nop");
      __asm__ volatile ("nop");
      __asm__ volatile ("nop");
      __asm__ volatile ("nop");
      __asm__ volatile ("nop");
      __asm__ volatile ("nop");
      __asm__ volatile ("nop");
    }
    duration--;
  }
}

void isr_systick(void) {
  uart_putchar(USART1, '.');
}

int main(void) {
  // We'll use UART0, so set it up
  pin_set_output_50mhz_alt(A, 9);
  uart_setup(USART1, 115200);

  uart_putline(USART1, "\r\nHello there!");

  // Set direction to output
  pin_set_output_50mhz(C, 13);

  // Let's also systick!
  interrupt_enable();
  SysTick->LOAD = F_CPU / 1;
  SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_CLKSOURCE_Msk;

  while (1) {
    pin_gpio_set_high(C, 13);
    uart_putchar(USART1, 'H');
    delay(500);
    pin_gpio_set_low(C, 13);
    uart_putchar(USART1, 'L');
    delay(500);
  }
}

