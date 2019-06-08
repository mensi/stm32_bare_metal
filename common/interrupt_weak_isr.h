#ifndef _INTERRUPT_WEAK_ISR_H_
#define _INTERRUPT_WEAK_ISR_H_

#ifndef _INTERRUPT_TABLE_SETUP_
#error "This file should only be included where the interrupt vector table is defined"
#endif

#include "interrupt.h"

/*
 * Interrupt service routine defaults.
 *
 * This header defines weak symbols for all ISRs and should therefore only be 
 * included by startup.c
 */

static void isr_panic_alias(void) {
  isr_panic();
}

static void isr_ignore_alias(void) {
  isr_ignore();
}

void isr_non_maskable(void)	    __attribute__ ((weak, alias("isr_panic_alias")));
void isr_hard_fault(void)	      __attribute__ ((weak, alias("isr_panic_alias")));
void isr_memmanage_fault(void)	__attribute__ ((weak, alias("isr_panic_alias")));
void isr_bus_fault(void)	      __attribute__ ((weak, alias("isr_panic_alias")));
void isr_usage_fault(void)	    __attribute__ ((weak, alias("isr_panic_alias")));

void isr_svcall(void)		        __attribute__ ((weak, alias("isr_ignore_alias")));
void isr_debug_monitor(void)	  __attribute__ ((weak, alias("isr_ignore_alias")));

void isr_pendablesrvreq(void)	  __attribute__ ((weak, alias("isr_ignore_alias")));
void isr_systick(void)		      __attribute__ ((weak, alias("isr_ignore_alias")));
void isr_window_watchdog(void)  __attribute__ ((weak, alias("isr_ignore_alias")));                             
void isr_pvd_exti(void)         __attribute__ ((weak, alias("isr_ignore_alias")));                      
void isr_tamper(void)           __attribute__ ((weak, alias("isr_ignore_alias")));                    
void isr_rtc(void)              __attribute__ ((weak, alias("isr_ignore_alias")));                 
void isr_flash(void)            __attribute__ ((weak, alias("isr_ignore_alias")));                   
void isr_rcc(void)              __attribute__ ((weak, alias("isr_ignore_alias")));                 
void isr_exti0(void)            __attribute__ ((weak, alias("isr_ignore_alias")));                   
void isr_exti1(void)            __attribute__ ((weak, alias("isr_ignore_alias")));                   
void isr_exti2(void)            __attribute__ ((weak, alias("isr_ignore_alias")));                   
void isr_exti3(void)            __attribute__ ((weak, alias("isr_ignore_alias")));                   
void isr_exti4(void)            __attribute__ ((weak, alias("isr_ignore_alias")));                   
void isr_dma1_ch1(void)         __attribute__ ((weak, alias("isr_ignore_alias")));                      
void isr_dma1_ch2(void)         __attribute__ ((weak, alias("isr_ignore_alias")));                      
void isr_dma1_ch3(void)         __attribute__ ((weak, alias("isr_ignore_alias")));                      
void isr_dma1_ch4(void)         __attribute__ ((weak, alias("isr_ignore_alias")));                      
void isr_dma1_ch5(void)         __attribute__ ((weak, alias("isr_ignore_alias")));                      
void isr_dma1_ch6(void)         __attribute__ ((weak, alias("isr_ignore_alias")));                      
void isr_dma1_ch7(void)         __attribute__ ((weak, alias("isr_ignore_alias")));                      
void isr_adc12(void)            __attribute__ ((weak, alias("isr_ignore_alias")));                   
void isr_usb_hp_can_tx(void)    __attribute__ ((weak, alias("isr_ignore_alias")));                           
void isr_usb_lp_can_rx0(void)   __attribute__ ((weak, alias("isr_ignore_alias")));                            
void isr_can_rx1(void)          __attribute__ ((weak, alias("isr_ignore_alias")));                     
void isr_can_sce(void)          __attribute__ ((weak, alias("isr_ignore_alias")));                     
void isr_exti_9_5(void)         __attribute__ ((weak, alias("isr_ignore_alias")));                      
void isr_tim1_brk(void)         __attribute__ ((weak, alias("isr_ignore_alias")));                      
void isr_tim1_up(void)          __attribute__ ((weak, alias("isr_ignore_alias")));                     
void isr_tim1_trg_com(void)     __attribute__ ((weak, alias("isr_ignore_alias")));                          
void isr_tim1_cc(void)          __attribute__ ((weak, alias("isr_ignore_alias")));                     
void isr_tim2(void)             __attribute__ ((weak, alias("isr_ignore_alias")));                  
void isr_tim3(void)             __attribute__ ((weak, alias("isr_ignore_alias")));                  
void isr_tim4(void)             __attribute__ ((weak, alias("isr_ignore_alias")));                  
void isr_i2c1_ev(void)          __attribute__ ((weak, alias("isr_ignore_alias")));                     
void isr_i2c1_er(void)          __attribute__ ((weak, alias("isr_ignore_alias")));                     
void isr_i2c2_ev(void)          __attribute__ ((weak, alias("isr_ignore_alias")));                     
void isr_i2c2_er(void)          __attribute__ ((weak, alias("isr_ignore_alias")));                     
void isr_spi1(void)             __attribute__ ((weak, alias("isr_ignore_alias")));                  
void isr_spi2(void)             __attribute__ ((weak, alias("isr_ignore_alias")));                  
void isr_usart1(void)           __attribute__ ((weak, alias("isr_ignore_alias")));                    
void isr_usart2(void)           __attribute__ ((weak, alias("isr_ignore_alias")));                    
void isr_usart3(void)           __attribute__ ((weak, alias("isr_ignore_alias")));                    
void isr_exti15_10(void)        __attribute__ ((weak, alias("isr_ignore_alias")));                       
void isr_rtcalarm(void)         __attribute__ ((weak, alias("isr_ignore_alias")));                      
void isr_usbwakeup(void)        __attribute__ ((weak, alias("isr_ignore_alias")));                       
void isr_tim8_brk(void)         __attribute__ ((weak, alias("isr_ignore_alias")));                      
void isr_tim8_up(void)          __attribute__ ((weak, alias("isr_ignore_alias")));                     
void isr_tim8_trg_com(void)     __attribute__ ((weak, alias("isr_ignore_alias")));                          
void isr_tim8_cc(void)          __attribute__ ((weak, alias("isr_ignore_alias")));                     
void isr_adc3(void)             __attribute__ ((weak, alias("isr_ignore_alias")));                  
void isr_fsmc(void)             __attribute__ ((weak, alias("isr_ignore_alias")));                  
void isr_sdio(void)             __attribute__ ((weak, alias("isr_ignore_alias")));                  
void isr_tim5(void)             __attribute__ ((weak, alias("isr_ignore_alias")));                  
void isr_spi3(void)             __attribute__ ((weak, alias("isr_ignore_alias")));                  
void isr_uart4(void)            __attribute__ ((weak, alias("isr_ignore_alias")));                   
void isr_uart5(void)            __attribute__ ((weak, alias("isr_ignore_alias")));                   
void isr_tim6(void)             __attribute__ ((weak, alias("isr_ignore_alias")));                  
void isr_tim7(void)             __attribute__ ((weak, alias("isr_ignore_alias")));                  
void isr_dma2_ch1(void)         __attribute__ ((weak, alias("isr_ignore_alias")));                      
void isr_dma2_ch2(void)         __attribute__ ((weak, alias("isr_ignore_alias")));                      
void isr_dma2_ch3(void)         __attribute__ ((weak, alias("isr_ignore_alias")));                      
void isr_dma2_ch4_5(void)       __attribute__ ((weak, alias("isr_ignore_alias")));                       

#endif
