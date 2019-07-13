#include "spi.h"
#include "config.h"
#include "uart.h"

void setup_spi_master(SPI_TypeDef * base, uint32_t baud, bool double_byte, bool lsb_first, bool software_slave_management) {
  uint32_t pclk = F_CPU;

  if (base == SPI1) {
    // Enable clock for SPI1
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;  
    
    // Setup output pins for default pin mapping
    if (!software_slave_management)
      pin_set_output_50mhz_alt(A, 4); // NSS
    pin_set_output_50mhz_alt(A, 5); // SCK
    pin_set_output_50mhz_alt(A, 7); // MOSI
  } else if (base == SPI2) {
    // TODO
  }

  // Reset CR1 to master mode
  base->CR1 = SPI_CR1_MSTR;

  // The SPI peripheral can either do 8 bits at a time or 16 bits
  if (double_byte) {
    base->CR1 |= SPI_CR1_DFF;
  }
  
  // By default, the order is MSB first but it can be reversed
  if (lsb_first) {
    base->CR1 |= SPI_CR1_LSBFIRST;
  }

  // The baud rate is derived from the peripheral clock, so we have to
  // select a divider that will bring us close to the target baud rate
  int brc = 0;
  uint32_t divided_clk = pclk >> 1;
  for (; brc < 7; brc++) {
    if (baud >= divided_clk) break;
    divided_clk >>= 1;
  }
  base->CR1 |= ((brc & 7) << SPI_CR1_BR_Pos) & SPI_CR1_BR_Msk;

  // Software Slave Management on and NSS high
  if (software_slave_management)
    base->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;

  base->CR1 |= SPI_CR1_SPE; // Enable
}

void spi_transfer_short(SPI_TypeDef * base, uint16_t data) {
  while (!(base->SR & SPI_SR_TXE));
  base->DR = data;
}

void spi_transfer_byte(SPI_TypeDef * base, uint8_t data) {
  while (!(base->SR & SPI_SR_TXE));
  base->DR = (uint16_t)data;
}

void spi_wait_until_not_busy(SPI_TypeDef * base) {
  while (base->SR & SPI_SR_BSY);
}