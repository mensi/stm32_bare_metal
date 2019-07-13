#include "pins.h"
#include "delay.h"
#include "uart.h"
#include "spi.h"

#define BITS(x) ((1U << x)-1)

typedef struct {
  unsigned char                   :  1; // MSB
  unsigned int integer            : 16;
  unsigned int fractional         : 12;
  unsigned char register_number   :  3; // LSB
} adf4351_reg0_t;
inline uint32_t adf4351_reg0_build(adf4351_reg0_t reg) {
    return (((uint32_t)reg.integer)    & BITS(16))  << 15 |
           (((uint32_t)reg.fractional) & BITS(12))  <<  3;
}
  
typedef struct {  
  unsigned char                   :  3; // MSB
  bool phase_adjust               :  1;
  bool precaler                   :  1;
  unsigned int phase              : 12;
  unsigned int modulus            : 12;
  unsigned char register_number   :  3; // LSB
} adf4351_reg1_t;
inline uint32_t adf4351_reg1_build(adf4351_reg1_t reg) {
    return (((uint32_t)reg.phase_adjust) & BITS(1))  << 28 |
           (((uint32_t)reg.precaler)     & BITS(1))  << 27 |
           (((uint32_t)reg.phase)        & BITS(12)) << 15 |
           (((uint32_t)reg.modulus)      & BITS(12)) <<  3 |
           1; // register number
}  
  
typedef struct {  
  unsigned char                   :  1; // MSB
  unsigned char low_noise_spur    :  2;
  unsigned char muxout            :  3;
  bool ref_doubler                :  1;
  bool rdiv2                      :  1;
  unsigned int rcounter           : 10;
  bool double_buffer              :  1;
  unsigned int charge_pump_cur    :  4;
  bool ldf                        :  1;
  bool ldp                        :  1;
  bool pd_polarity                :  1;
  bool power_down                 :  1;
  bool cp_threestate              :  1;
  bool counter_reset              :  1;
  unsigned char register_number   :  3; // LSB 
} adf4351_reg2_t;
inline uint32_t adf4351_reg2_build(adf4351_reg2_t reg) {
    return (((uint32_t)reg.low_noise_spur)  & BITS(2))  << 29 |
           (((uint32_t)reg.muxout)          & BITS(3))  << 26 |
           (((uint32_t)reg.ref_doubler)     & BITS(1))  << 25 |
           (((uint32_t)reg.rdiv2)           & BITS(1))  << 24 |
           (((uint32_t)reg.rcounter)        & BITS(10)) << 14 |
           (((uint32_t)reg.double_buffer)   & BITS(1))  << 13 |
           (((uint32_t)reg.charge_pump_cur) & BITS(4))  <<  9 |
           (((uint32_t)reg.ldf)             & BITS(1))  <<  8 |
           (((uint32_t)reg.ldp)             & BITS(1))  <<  7 |
           (((uint32_t)reg.pd_polarity)     & BITS(1))  <<  6 |
           (((uint32_t)reg.power_down)      & BITS(1))  <<  5 |
           (((uint32_t)reg.cp_threestate)   & BITS(1))  <<  4 |
           (((uint32_t)reg.counter_reset)   & BITS(1))  <<  3 |
           2; // register number
}

typedef struct {
  unsigned char                   :  8; // MSB
  bool band_select_clock_mode     :  1;
  bool abp                        :  1; // 0 = 6ns ABP, recommended for fractional-N mode. 1 = 3ns ABP, recommended for integer-N mode.
  bool charge_cancel              :  1;
  unsigned char                   :  2;
  bool csr                        :  1;
  bool                            :  1;
  unsigned char clk_div_mode      :  2;
  unsigned int clock_divider      : 12;
  unsigned char register_number   :  3; // LSB 
} adf4351_reg3_t;
inline uint32_t adf4351_reg3_build(adf4351_reg3_t reg) {
    return (((uint32_t)reg.band_select_clock_mode)  & BITS(1))  << 23 |
           (((uint32_t)reg.abp)                     & BITS(1))  << 22 |
           (((uint32_t)reg.charge_cancel)           & BITS(1))  << 21 |
           (((uint32_t)reg.csr)                     & BITS(1))  << 18 |
           (((uint32_t)reg.clk_div_mode)            & BITS(2))  << 15 |
           (((uint32_t)reg.clock_divider)           & BITS(12)) <<  3 |
           3; // register number
}

typedef struct {
  unsigned char                   :  8; // MSB
  bool feedback_select            :  1;
  unsigned char rf_divider_select :  3;
  unsigned char band_select       :  8;
  bool vco_powerdown              :  1;
  bool mtld                       :  1;
  bool aux_output_select          :  1;
  bool aux_output_enable          :  1;
  unsigned char aux_output_power  :  2;
  bool rf_output_enable           :  1;
  unsigned char output_power      :  2;
  unsigned char register_number   :  3; // LSB
} adf4351_reg4_t;
inline uint32_t adf4351_reg4_build(adf4351_reg4_t reg) {
    return (((uint32_t)reg.feedback_select)   & BITS(1)) << 23 |
           (((uint32_t)reg.rf_divider_select) & BITS(3)) << 20 |
           (((uint32_t)reg.band_select)       & BITS(8)) << 12 |
           (((uint32_t)reg.vco_powerdown)     & BITS(1)) << 11 |
           (((uint32_t)reg.mtld)              & BITS(1)) << 10 |
           (((uint32_t)reg.aux_output_select) & BITS(1)) <<  9 |
           (((uint32_t)reg.aux_output_enable) & BITS(1)) <<  8 |
           (((uint32_t)reg.aux_output_power)  & BITS(2)) <<  6 |
           (((uint32_t)reg.rf_output_enable)  & BITS(1)) <<  5 |
           (((uint32_t)reg.output_power)      & BITS(2)) <<  3 |
           4; // register number
}

typedef struct {
  unsigned char                   :  8;
  unsigned char ld_pin_mode       :  2;
  unsigned int                    : 19;
  unsigned char register_number   :  3; 
} adf4351_reg5_t;
inline uint32_t adf4351_reg5_build(adf4351_reg5_t reg) {
    return (((uint32_t)reg.ld_pin_mode) & BITS(2)) << 22 |
           3U << 19 | // Reserved bits DB20 and DB19 are 1
           5; // register number
}

void print_register(uint32_t value) {
  int register_index = value & 7;
  usart1_printf("r%d = 0x%x", register_index, value);
}

void write_register(uint32_t value) {
  pin_gpio_set_low(A, 4);
  delay_us(1);
  
  spi_transfer_byte(SPI1, (value >> 24) & 0xFF);
  spi_transfer_byte(SPI1, (value >> 16) & 0xFF);
  spi_transfer_byte(SPI1, (value >> 8) & 0xFF);
  spi_transfer_byte(SPI1, (value) & 0xFF);
  
  spi_wait_until_not_busy(SPI1);
  
  delay_us(1);
  pin_gpio_set_high(A, 4);
}

int main(void) {
  usart1_setup_default();
  usart1_putline("\r\nADF 4351 on A5: SCK, A7: MOSI, A4: LE");

  pin_set_output_50mhz(A, 4);
  pin_gpio_set_high(A, 4);
  
  setup_spi_master(SPI1, 1000000, false, false, true);

  adf4351_reg0_t r0 = {
    .integer = 75,   // Just set the lowest allowed value for both prescalers as default
    .fractional = 0
  };

  adf4351_reg1_t r1 = {
    .phase_adjust = 0, // Setting to 1 disables VCO band selection when register 0 is updated.
    .precaler = 1,     // Set prescaler to 8/9 which can reach above 3.6 GHz
    .phase = 1,        // As per recommendation in datasheet
    .modulus = 0
  };

  adf4351_reg2_t r2 = {
    .low_noise_spur = 0, // Low noise mode (set to 3 for low spur mode)
    .muxout = 0,
    .ref_doubler = 0,
    .rdiv2 = 0,          // Don't divide reference by 2
    .rcounter = 1,       // Don't divide clock input
    .double_buffer = 0,
    .charge_pump_cur = 7, // Charge pump current: 2.5 mA
    .ldf = 0,
    .ldp = 0,
    .pd_polarity = 0,
    .power_down = 0,
    .cp_threestate = 0,
    .counter_reset = 0
  };

  adf4351_reg3_t r3 = {
    .band_select_clock_mode = 0, // 0 = slow, 1 = fast
    .abp = 0,
    .charge_cancel = 0,
    .csr = 0,
    .clk_div_mode = 0, // 0 = disable. 10 = phase resync. 01 = fast lock
    .clock_divider = 150 // This value is the timeout counter for activation of phase resync (see the Phase Resync section).
  };

  adf4351_reg4_t r4 = {
    .feedback_select = 1,  // 1 = Take VCO output for N counter, 0 = take output of output dividers
    .rf_divider_select = 1,
    .band_select = 0, 
    .vco_powerdown = 0,
    .mtld = 1,               // Mute the RF output until lock is detected
    .aux_output_select = 0,
    .aux_output_enable = 0,
    .aux_output_power = 0,
    .rf_output_enable = 0,
    .output_power = 3
  };

  adf4351_reg5_t r5 = {
    .ld_pin_mode = 1
  };

  // Fvco = Fpfd * (INT + FRAC/MOD)
  // Fout = Fvco / output_divider
  // INT can be 23 to 65535 for 4/5 prescaler, 75 to 65535 for 8/9 precaler
  // FRAC can be 0 to MOD - 1
  // MOD can be 2 to 4096

  double f_out = 40000000; // 50 MHz target output frequency
  double f_refin = 25000000; // 25 MHz oscillator at REFin

  // Fpfd = Frefin * [(1 + D)/(R * (1 + T))]  where D = doubler bit and T = divide by 2 bit
  double f_pfd = f_refin * ((r2.ref_doubler ? 2.0d : 1.0d) / (r2.rcounter * (r2.rdiv2 ? 2.0d : 1.0d)));

  // The clock band selector logic is clocked from Fpfd through a divider.
  // It can be up to 125kHz with the slow logic and up to 500kHz with the fast one.
  // Reference:
  //  Setting the DB23 bit to 0 is
  //  recommended for low PFD (<125 kHz) values. For the faster
  //  band select logic modes (DB23 set to 1), the value of the band
  //  select clock divider must be less than or equal to 254.
  int band_select_divisor = 0;
  if (r3.band_select_clock_mode) { // fast -> 500 kHz
    band_select_divisor = (int)(f_pfd / 500000);
    if (band_select_divisor > 254) {
      usart1_printf("WARNING: Clock divisor in fast mode too high: %d", band_select_divisor);
      band_select_divisor = 254;
    }
  } else { // slow -> 125 kHz
    band_select_divisor = (int)(f_pfd / 125000);
    if (band_select_divisor > 255) {
      usart1_printf("WARNING: Clock divisor in slow mode too high: %d", band_select_divisor);
      band_select_divisor = 255;
    }
  }
  r4.band_select = band_select_divisor;
  double f_band_select = f_pfd / band_select_divisor;

  usart1_printf("band_selector divisor: %d, clock: %f kHz", band_select_divisor, f_band_select / 1000.0d);

  // VCO has to be between 2.2 and 4.4 GHz. It is divided for the output
  //  -> find a divider that will get us in that range
  double f_vco = 0.0d;
  for (r4.rf_divider_select = 0; r4.rf_divider_select < 8; r4.rf_divider_select++) {
    f_vco = (1 << r4.rf_divider_select) * f_out;
    if (f_vco >= 2200000000 && f_vco <= 4400000000) break;
  }
    
  usart1_printf("out: %f MHz, refin: %f MHz, pfd: %f MHz, vco: %f MHz", f_out / 1000000.0, f_refin / 1000000.0, f_pfd / 1000000.0, f_vco / 1000000.0);

  // If feedback select 1 = Take VCO output for N counter, 0 = take output of output dividers
  double N = r4.feedback_select ? f_vco / f_pfd : f_out / f_pfd;

  // For the PLL, we have an integer part we use to divide VCO to get to FPFD
  r0.integer = (uint32_t)N;

  // The modulus will define the resolution
  r1.modulus = (uint32_t)(f_pfd / 1000.0); 

  // We can now compute the fractional part as we have all the rest
  r0.fractional = (uint32_t)(r1.modulus * (N - (double)r0.integer));

  if (r0.fractional == 0) { // Integer-N mode. 
    // LDF in reg 2 should be set to 1
    r2.ldf = 1;

    // LDP for integer-N applications, the recommended setting for Bits[DB8:DB7] is 11. 
    r2.ldp = 3;

    // ABP in reg 3 should be set to 1
    r3.abp = 1;

    // This has the effect of reducing PFD spurs in integer-N mode.
    r3.charge_cancel = 1;

    // Fpfd can be up to 90 MHz
    if (f_pfd > 90000000) {
      usart1_printf("WARNING: Fpfd > 90 MHz: %f", f_pfd);
    }

    // Fpfd higher than 45 MHz should set phase adjust in register 1 to 1. But this disables update on R0 writes.
  } else { // Fractional-N mode
    // LDF in reg 2 should be set to 0
    r2.ldf = 0;

    // LDP for fractional-N applications, the recommended setting for Bits[DB8:DB7] is 00; 
    r2.ldp = 0;

    // ABP in reg 3 should be set to 0
    r3.abp = 0;

    // In fractional-N mode, this bit should be set to 0.
    r3.charge_cancel = 0;
  }

  // Enable out
  r4.rf_output_enable = 1;

  usart1_printf("Int: %d, Frac: %d, mod: %d", r0.integer, r0.fractional, r1.modulus);
  usart1_printf("Effective RFout: %f MHz", ((double)r0.integer + (double)r0.fractional / (double)r1.modulus) * f_pfd / (double)(1 << r4.rf_divider_select) / 1000000.0d);

  while (1) {
    usart1_putline("Start Update");

    print_register(adf4351_reg5_build(r5));
    print_register(adf4351_reg4_build(r4));
    print_register(adf4351_reg3_build(r3));
    print_register(adf4351_reg2_build(r2));
    print_register(adf4351_reg1_build(r1));
    print_register(adf4351_reg0_build(r0));

    write_register(adf4351_reg5_build(r5));
    write_register(adf4351_reg4_build(r4));
    write_register(adf4351_reg3_build(r3));
    write_register(adf4351_reg2_build(r2));
    write_register(adf4351_reg1_build(r1));
    write_register(adf4351_reg0_build(r0));

    usart1_putline("Done");
    delay_ms(10000);
  }
}
