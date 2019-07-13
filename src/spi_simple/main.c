#include "pins.h"
#include "delay.h"
#include "uart.h"

int main(void) {
  usart1_setup_default();
  usart1_putline("\r\nspi_simple on A4: NSS, A5: SCK, A6: MISO, A7: MOSI");

  // Enable clock for SPI1
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

  // Setup output pins
  pin_set_output_50mhz_alt(A, 4); // NSS
  pin_set_output_50mhz_alt(A, 5); // SCK
  pin_set_output_50mhz_alt(A, 7); // MOSI

  SPI1->CR1 = SPI_CR1_DFF | SPI_CR1_MSTR | SPI_CR1_BR_2; // Master mode
  SPI1->CR1 |= SPI_CR1_SPE; // Enable

  while (1) {
    while (!(SPI1->SR & SPI_SR_TXE));
    SPI1->DR = ('H' << 8) | 'e';
    while (!(SPI1->SR & SPI_SR_TXE));
    SPI1->DR = ('l' << 8) | 'l';
    while (!(SPI1->SR & SPI_SR_TXE));
    SPI1->DR = ('o' << 8) | '!';
    
    while (SPI1->SR & SPI_SR_BSY);
    delay_ms(500);
  }
}
