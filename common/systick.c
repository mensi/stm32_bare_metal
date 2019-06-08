#include "config.h"
#include "systick.h"
#include "core_cm3.h"

#define F_SYSTICK 10

volatile unsigned int systick_ticks = 0;

void systick_init() {
  SysTick->LOAD = F_CPU / F_SYSTICK;
  SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_CLKSOURCE_Msk;
}

unsigned int systick_get_ms() {
  return systick_ticks * (1000 / F_SYSTICK) + 
    (F_CPU / F_SYSTICK - (SysTick->VAL & SysTick_VAL_CURRENT_Msk)) / (F_CPU/1000);
}

#if ENABLE_SYSTICK_HANDLER
void isr_systick(void) {
  systick_ticks++;
}
#endif
