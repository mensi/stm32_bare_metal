#!/usr/bin/env python

def pin_iteration(fmt):
    """Iterate over the pinmap, yielding fmt formatted with idx, bank and pin."""
    for i, (bank, pin) in enumerate(pinmap):
        yield fmt.format(idx=i, bank=bank, pin=pin)

def strip(line):
    """Strip the identation of a line for the comment header."""
    if line.startswith(' ' * 4):
        line = line[4:]
    return line.rstrip()

def generate_contents():
    hdr = '''
    This is a generated file. Do not modify directly!
    -------------------------------------------------

    This file contains macros for more convenient register access. 
    
    - PIN_GPIO_CRXx
       CRL or CRH for pin x

    - PIN_GPIO_CRX_CNFx
       The GPIO CRL/CRH CNF bit for pin x

    - PIN_GPIO_CRX_MODE
       The GPIO CRL/CRH MODE bit for pin x

    "Function"-like macros for convenience:

    - pin_gpio_set_high(port, idx)
       Sets the GPIO pin to HIGH.

    - pin_gpio_set_low(port, idx)
       Sets the GPIO pin to LOW.
    '''
    # Output file header
    lines = ['#ifndef _PINS_H_', '#define _PINS_H_', '#include "stm32f103c8t6.h"']
    lines += ['/*'] + [' * ' + strip(line) for line in hdr.lstrip().split('\n')] + [' */']

    def section(title):
        """Yield an empty line and a section title comment."""
        yield ''
        yield '// ' + title

    lines += section('GPIO set high/low')
    lines.append('#define pin_gpio_set_high(port, idx) GPIO##port->BSRR = GPIO_BSRR_BS##idx')
    lines.append('#define pin_gpio_set_low(port, idx) GPIO##port->BSRR = GPIO_BSRR_BR##idx')

    lines += section('CRL vs. CRH for each pin number')
    for idx in range(16):
        lines.append('#define PIN_GPIO_CRX{} {}'.format(idx, 'CRL' if idx < 8 else 'CRH'))

    lines += section('GPIO_CRx_CNF')
    for idx in range(16):
        lines.append('#define PIN_GPIO_CRX_CNF{0} GPIO_{1}_CNF{0}'.format(idx, 'CRL' if idx < 8 else 'CRH'))
        lines.append('#define PIN_GPIO_CRX_CNF{0}_0 GPIO_{1}_CNF{0}_0'.format(idx, 'CRL' if idx < 8 else 'CRH'))
        lines.append('#define PIN_GPIO_CRX_CNF{0}_1 GPIO_{1}_CNF{0}_1'.format(idx, 'CRL' if idx < 8 else 'CRH'))

    lines += section('GPIO_CRx_MODE')
    for idx in range(16):
        lines.append('#define PIN_GPIO_CRX_MODE{0} GPIO_{1}_MODE{0}'.format(idx, 'CRL' if idx < 8 else 'CRH'))
        lines.append('#define PIN_GPIO_CRX_MODE{0}_0 GPIO_{1}_MODE{0}_0'.format(idx, 'CRL' if idx < 8 else 'CRH'))
        lines.append('#define PIN_GPIO_CRX_MODE{0}_1 GPIO_{1}_MODE{0}_1'.format(idx, 'CRL' if idx < 8 else 'CRH'))

    lines += section('Pin output configuration / setup')
    lines.append('#define pin_set_input_analog(port, idx) GPIO##port->PIN_GPIO_CRX##idx = (GPIO##port->PIN_GPIO_CRX##idx & ~( PIN_GPIO_CRX_CNF##idx  | PIN_GPIO_CRX_MODE##idx ))')
    lines.append('#define pin_set_output_50mhz(port, idx) GPIO##port->PIN_GPIO_CRX##idx = (GPIO##port->PIN_GPIO_CRX##idx & ~( PIN_GPIO_CRX_CNF##idx  | PIN_GPIO_CRX_MODE##idx )) | (PIN_GPIO_CRX_MODE##idx##_1 | PIN_GPIO_CRX_MODE##idx##_0)')
    lines.append('#define pin_set_output_50mhz_alt(port, idx) GPIO##port->PIN_GPIO_CRX##idx = (GPIO##port->PIN_GPIO_CRX##idx & ~( PIN_GPIO_CRX_CNF##idx  | PIN_GPIO_CRX_MODE##idx )) | (PIN_GPIO_CRX_MODE##idx##_1 | PIN_GPIO_CRX_MODE##idx##_0 | PIN_GPIO_CRX_CNF##idx##_1)')

    lines += ['#endif']
    return lines

def print_contents():
    for line in generate_contents():
        print(line)

if __name__ == '__main__':
    print_contents()
