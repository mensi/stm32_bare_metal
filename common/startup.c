#define _INTERRUPT_TABLE_SETUP_
/*
 * This file contains everything necessary to boot and jump to main()
 * 
 * It's structured in the same sequence sections will appear on flash
 */

#include "config.h"
#include "interrupt.h"
#include "interrupt_weak_isr.h"
#include "systick.h"
#include "watchdog.h"
#include "stm32f103c8t6.h"

/*
 * Initial stack pointer. _eram will be set in the linker script to be
 * the end of the RAM. The stack grows "upwards", so that's where we 
 * want to start the stack.
 */
extern unsigned long _eram;

/*
 * Other linker symbols
 */
extern unsigned long _etext;
extern unsigned long _sdata;
extern unsigned long _edata;
extern unsigned long _sbss;
extern unsigned long _ebss;

/*
 * Main function we will eventually call after startup
 */
void main(void);

/*
 * Initial program counter. The address where we want to start execution.
 * The actual function is implemented further down.
 */
void __startup(void);

/*
 * Initial interrupt vector table, of which the first two entries are 
 * stack pointer and initial PC.
 */
__attribute__ ((section(".vectors"), used))
void (* const interrupt_vectors[])(void) =
{
  (void (*)(void))((unsigned long)&_eram),
  __startup,
  isr_non_maskable,
  isr_hard_fault,
  isr_memmanage_fault,
  isr_bus_fault,
  isr_usage_fault,
  isr_ignore,  // Reserved 7
  isr_ignore,  // Reserved 8
  isr_ignore,  // Reserved 9
  isr_ignore,  // Reserved 10
  isr_svcall,
  isr_debug_monitor,
  isr_ignore,  // Reserved 13
  isr_pendablesrvreq,
  isr_systick,
  isr_window_watchdog,
  isr_pvd_exti,
  isr_tamper,
  isr_rtc,
  isr_flash,
  isr_rcc,
  isr_exti0,
  isr_exti1,
  isr_exti2,
  isr_exti3,
  isr_exti4,
  isr_dma1_ch1,
  isr_dma1_ch2,
  isr_dma1_ch3,
  isr_dma1_ch4,
  isr_dma1_ch5,
  isr_dma1_ch6,
  isr_dma1_ch7,
  isr_adc12,
  isr_usb_hp_can_tx,
  isr_usb_lp_can_rx0,
  isr_can_rx1,
  isr_can_sce,
  isr_exti_9_5,
  isr_tim1_brk,
  isr_tim1_up,
  isr_tim1_trg_com,
  isr_tim1_cc,
  isr_tim2,
  isr_tim3,
  isr_tim4,
  isr_i2c1_ev,
  isr_i2c1_er,
  isr_i2c2_ev,
  isr_i2c2_er,
  isr_spi1,
  isr_spi2,
  isr_usart1,
  isr_usart2,
  isr_usart3,
  isr_exti15_10,
  isr_rtcalarm,
  isr_usbwakeup,
  isr_tim8_brk,
  isr_tim8_up,
  isr_tim8_trg_com,
  isr_tim8_cc,
  isr_adc3,
  isr_fsmc,
  isr_sdio,
  isr_tim5,
  isr_spi3,
  isr_uart4,
  isr_uart5,
  isr_tim6,
  isr_tim7,
  isr_dma2_ch1,
  isr_dma2_ch2,
  isr_dma2_ch3,
  isr_dma2_ch4_5
};

/*
 * Reset handler code. This will be executed when the MCU
 * comes out of reset.
 */
__attribute__ ((section(".startup"), optimize("-Os")))
void __startup(void) {
  // For the sake of simplicity, enable all port clocks
  RCC->APB2ENR |= (RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPDEN);
  
  // ----------------------------------------------------------------------------------
  // Setup clocks
  // ----------------------------------------------------------------------------------
  // See section 7.2 in RM0008 for the different clocks / the clock tree.
  // The limits are:
  //   System Clock (SYSCLK):  max 72 MHz
  //   USB Clock:              needs to be 48 MHz, only achievable with 72 and 48 MHz SYSCLK 
  //                           due to the 1 or 1.5 prescaler
  //   APB1:                   max 36 MHz (SYSCLK divide by 1, 2, 4, 8, 16)
  //   APB2:                   max 72 MHz
  //   ADC:                    max 14 MHz (SYSCLK divide by 2, 4, 6, 8)
  //   Flash:                  is a bit special, depending on clock ranges, there is a 
  //                           defined amount of wait states:
  //                             0 if SYSCLK is from 0 up to 24 MHz
  //                             1 if SYSCLK is over 24 up to 48 MHz
  //                             2 if SYSCLK is over 48 up to 72 MHz
  
  // For now, we'll assume you always want to have USB, so only 48 and 72 MHz make sense.
  // Also, assume we have an external 8 MHz crystal on the HSE.

  // First, set the divisors for slower clock domains.
  #if F_CPU == 48000000
    FLASH->ACR |= FLASH_ACR_LATENCY_1; // 1 wait state for flash
    RCC->CFGR |= (
        RCC_CFGR_PPRE1_DIV2 // APB1: Divide by 2
      | RCC_CFGR_USBPRE  // Do not divide PLL clock for USB
    );
  #elif F_CPU == 72000000
    FLASH->ACR |= FLASH_ACR_LATENCY_2; // 2 wait states for flash
    RCC->CFGR |= (
      RCC_CFGR_PPRE1_DIV2 // APB1: Divide by 2
    );
  #else
    #error "Unknown F_CPU value"
  #endif

  // Then, turn the HSE clock on and wait for it to be ready
  RCC->CR |= RCC_CR_HSEON;
  while ((RCC->CR & RCC_CR_HSERDY) == 0) ;

  // Setup the PLL
  RCC->CFGR |= RCC_CFGR_PLLSRC; // Switch from HSI/2 to the HSE clock for the PLL clock source.
  #if F_CPU == 48000000
    RCC->CFGR |= RCC_CFGR_PLLMULL6; // Multiply by 6
  #elif F_CPU == 72000000
    RCC->CFGR |= RCC_CFGR_PLLMULL9; // Multiply by 9
  #else
    #error "Unknown F_CPU value"
  #endif

  // Turn PLL on and wait for it
  RCC->CR |= RCC_CR_PLLON;
  while ((RCC->CR & RCC_CR_PLLRDY) == 0) ;

  // Switch SYSCLK over to PLL and wait for it
  RCC->CFGR |= RCC_CFGR_SW_PLL;
  while ((RCC->CFGR & RCC_CFGR_SWS_PLL) == 0) ;

  // ----------------------------------------------------------------------------------
  // Relocate data from flash to RAM as necessary
  // ----------------------------------------------------------------------------------
  //
  // At the minimum, the .data and .bss sections have to be setup in RAM. Also, since
  // they are aligned to 4 bytes, we can use uint32s for copying (which is faster than
  // byte by byte)
  uint32_t * src = &_etext;
  uint32_t * dest = &_sdata;
  while (dest < &_edata) *dest++ = *src++;

  // Also zero out .bss
  dest = &_sbss;
  while (dest < &_ebss) *dest++ = 0;

  // TODO: Relocate interrupt vector to RAM for speed?


  // Init systick?
#if ENABLE_SYSTICK_HANDLER
  systick_init();
#endif

  // Enable interrupts before entering main?
#if ENABLE_INTERRUPTS_ON_STARTUP
  interrupt_enable();
#endif

  // After everything is done, call main
  main(); 

  // This should be unreachable code as long as main() does not return.
  // To avoid running the instruction pointer into places it shouldn't go, 
  // loop forever
  // TODO: Going into sleep would maybe be a better solution
  while (1);
}
