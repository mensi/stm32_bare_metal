#ifndef _PINS_H_
#define _PINS_H_
#include "stm32f103c8t6.h"

#define pin_set_output_50mhz(port, idx) GPIO##port->CRH = (GPIO##port->CRH & ~( GPIO_CRH_CNF##idx  | GPIO_CRH_MODE##idx )) | (GPIO_CRH_MODE##idx##_1 | GPIO_CRH_MODE##idx##_0)
#define pin_set_output_50mhz_alt(port, idx) GPIO##port->CRH = (GPIO##port->CRH & ~( GPIO_CRH_CNF##idx  | GPIO_CRH_MODE##idx )) | (GPIO_CRH_MODE##idx##_1 | GPIO_CRH_MODE##idx##_0 | GPIO_CRH_CNF##idx##_1)

#define pin_gpio_set_high(port, idx) GPIO##port->BSRR = GPIO_BSRR_BS##idx
#define pin_gpio_set_low(port, idx) GPIO##port->BSRR = GPIO_BSRR_BR##idx

#endif
