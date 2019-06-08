#include "uart.h"
#include "config.h"

int uart_get_port_index(USART_TypeDef * base) {
  if (base == USART1)
    return 1;
  else if (base == USART2)
    return 2;

  // So, if we get here, the base pointer is off.
  // For now, just go into an endless loop until we have
  // a better failure mode.
  while (1);
}

void uart_setup(USART_TypeDef * base, int baud) {
  // First off, enable the peripherial clock for the UART
  switch (uart_get_port_index(base)) {
    case 1: RCC->APB2ENR |= RCC_APB2ENR_USART1EN; break;
    case 2: RCC->APB1ENR |= RCC_APB1ENR_USART2EN; break;
  }

  // First, we would have to set up the word length, parity bits and stop bits.
  // But since we want 8n1, the defaults of 0 are already correct.
  base->CR1 = 0;
  base->CR2 = 0;
  base->CR3 = 0;

  // Set the baud rate which is derived by dividing the module clock:
  //  baud = module_clock / (16 * divisor)
  // The divisor is encoded in BRR as a fixed point number with
  // 12 bits of mantissa and 4 bits of fraction.
  // Since there are 4 fraction bits and the divisor is multiplied by 16,
  // we can just do an integer division and it will work out.
  base->BRR = (uint32_t)(F_CPU/baud); // TODO: This only works for USART1 which is on the high speed bus
  
  // Enable Tx
  base->CR1 |= USART_CR1_TE | USART_CR1_UE;
}

void uart_putchar(USART_TypeDef * base, char ch) {
  // Wait until space is available
  while(!(base->SR & USART_SR_TXE_Msk));

  // Write character
  base->DR = (uint8_t)ch;
}

void uart_putline(USART_TypeDef * base, char * str) {
  while (*str != '\0') {
    uart_putchar(base, *str);
    str++;
  }

  uart_putchar(base, '\r');
  uart_putchar(base, '\n');
}