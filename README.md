# Bare-metal Code Examples for STM32 microcontrollers
This repository contains bare-metal C code written from scratch based on the reference manuals.
It's aimed to be heavily commented to allow a smooth start for C-developers diving into embedded coding with STM32 based
boards like the Blue Pill.

For the register address defines, [CMSIS](https://developer.arm.com/tools-and-software/embedded/cmsis) headers from ARM and
STMicroelectronics are used. CMSIS also contains startup code, linker scripts and even higher abstractions which you might
want to use for production grade applications. These are not used in this repository in order to demonstrate programming
at a low level.

The code is split into two parts:

 * **common** contains general-purpose/framework code, Makefile, linker script etc. shared amongst the different examples.
 * **src** has subdirectories with different examples.

To start experimenting yourself, simply create a new subfolder in *src* and create a symlink *Makefile -> ../../common/Makefile*.

## Prerequisites

 * GNU make
 * GCC arm-none-eabi
 * CMake (for stlink)
 * libusb 1.0 (for stlink)

On Debian/Ubuntu, the following should install the necessary requirements:

```bash
apt-get install build-essential gcc-arm-none-eabi cmake libusb-1.0-0-dev
```

If you did not use `--recursive` when cloning, don't forget to init submodules (for stlink):

```bash
git submodule init
git submodule update
```

You should now be able to run `make build` in any of the examples in *src*. Use `make load` to also load the resulting hex file with st-flash.

## Reading Material

 * [STM32F103x8, STM32F103xB datasheet](https://www.st.com/resource/en/datasheet/stm32f103c8.pdf)
 * [PM0075 Programming manual - STM32F10xxx Flash memory microcontrollers](https://www.st.com/content/ccc/resource/technical/document/programming_manual/10/98/e8/d4/2b/51/4b/f5/CD00283419.pdf/files/CD00283419.pdf/jcr:content/translations/en.CD00283419.pdf)
 * [RM0008 Reference manual - STM32F101xx, STM32F102xx, STM32F103xx, STM32F105xx and STM32F107xx advanced Arm®-based 32-bit MCUs](https://www.st.com/content/ccc/resource/technical/document/reference_manual/59/b9/ba/7f/11/af/43/d5/CD00171190.pdf/files/CD00171190.pdf/jcr:content/translations/en.CD00171190.pdf)
