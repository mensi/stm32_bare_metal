#ifndef _UART_H_
#define _UART_H_

#include "pins.h"

void uart_setup(USART_TypeDef * base, int baud);
void uart_putchar(USART_TypeDef * base, char ch);
void uart_putline(USART_TypeDef * base, char * str);


/*
 * Convenience function to setup USART1 on PA9 and PA10 with 115200 baud
 */
inline static void usart1_setup_default() {
  pin_set_output_50mhz_alt(A, 9);
  // Set PA10 input floating?
  
  uart_setup(USART1, 115200);
}

inline static void usart1_putchar(char ch) {
  uart_putchar(USART1, ch);
}

inline static void usart1_putline(char * str) {
  uart_putline(USART1, str);
}

#endif
