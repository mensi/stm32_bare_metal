#include "stm32f103c8t6.h"

void delay(unsigned int duration) {
  // Hacky approximation: We're running at 72 MHz by default, so let's
  // just take something around 10 cycles long and do it 72000/10 times
  // to wait one millisecond. With interrupts disabled, this should be
  // in the ballpark of real time.
  while (duration > 0) {
    int inner = 72000 / 10;
    while (inner--) {
      __asm__ volatile ("nop");
      __asm__ volatile ("nop");
      __asm__ volatile ("nop");
      __asm__ volatile ("nop");
      __asm__ volatile ("nop");
      __asm__ volatile ("nop");
      __asm__ volatile ("nop");
    }
    duration--;
  }
}

int main(void) {
  // Note: For this to work, we have to enable the port clock for the pin
  //       we are trying to use. startup.c enables all port clocks so it should be fine.

  // Port C pin 13 (with the onboard LED)
  GPIOC->CRH = (0x44444444 &                      // Reset value (see RM0008 Section 9.2.2)
    ~(GPIO_CRH_MODE13_Msk | GPIO_CRH_CNF13_Msk))  // Clear out CNF and MODE for pin 13
    | (GPIO_CRH_MODE13_1 | GPIO_CRH_MODE13_0);    // Set mode to 11 = Output mode, max speed 50 MHz
  
  // For setting the output value, we have either the option of writing the
  // GPIOx_ODR register directly - this register has 1 bit for each pin. However,
  // this is not so great if we also have inputs or other outputs on the same port
  // as it gets tricky to not mess up adjacent values.
  // So instead, we can use the GPIOx_BSRR register instead wich lets us set individual
  // bits atomically. See section 9.2.5 in RM0008.
  while (1) {
    GPIOC->BSRR = GPIO_BSRR_BS13; // Set GPIO C pin 13 (make it HIGH)
    delay(500);
    GPIOC->BSRR = GPIO_BSRR_BR13; // Clear GPIO C pin 13 (make it LOW)
    delay(500);
  }
}

