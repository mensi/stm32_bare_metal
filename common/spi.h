#ifndef _SPI_H_
#define _SPI_H_

#include <stdbool.h>
#include "pins.h"

void setup_spi_master(SPI_TypeDef * base, uint32_t baud, bool double_byte, bool lsb_first, bool software_slave_management);
void spi_transfer_short(SPI_TypeDef * base, uint16_t data);
void spi_transfer_byte(SPI_TypeDef * base, uint8_t data);
void spi_wait_until_not_busy(SPI_TypeDef * base);

#endif
