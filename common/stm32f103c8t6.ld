/*
 * First off, let's define the regions for flash and RAM.
 * The STM32F103C8T6 has 64K of flash starting at 0x08000000
 *
 * The RAM is 20K starting at 0x20000000.
 */

MEMORY
{
  FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 64K
  RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 20K
}

SECTIONS
{
  .text : {
    /* The .text section is placed at the start in flash, which starts at address 0 */
    . = 0;

    /* First up is the interrupt vectors. The first two entries are even more special,
     * as they hold the initial stack pointer and the initial program counter.
     * Mark this section KEEP to prevent it being garbage collected.
     * The syntax *(.foo) means take the .foo sections from all files (*). */
    KEEP(*(.vectors))

    /* Next, add the startup code */
    *(.startup*)

    /* Now let's load all the code */
    *(.text*)

    /* And read-only data */
    *(.rodata*)
  } > FLASH = 0xFF /* Fill the flash with 0xFF as this is the default, unprogrammed state */

  /* This is the end of the data that will remain in flash. We need to
   * keep this for later for any sections that are loaded in flash but
   * need to be relocated to RAM */
  . = ALIGN(4);
  _etext = .;


  /* Data contains all the (mutable) variables with predefined data.
   * Therefore, we need to put it in flash but the code should refer
   * to addresses in RAM.
   * AT (_etext) places the "payload" after everything we put in flash
   * so far, but > RAM puts this section in RAM for linking.
   *
   * This means that early during the boot, we have to copy this from
   * flash to ram at the provided locations. We therefore need to 
   * save the start and end (_sdata, _edata) for the memcopy. 
   *
   * We also align start and end to 4 byte = 32bit boundaries so we can
   * simply copy whole registers/int32 instead of byte per byte */
  .data : AT (_etext) {
    . = ALIGN(4);
    _sdata = .; 
    *(.data*)
    . = ALIGN(4);
    _edata = .; 
  } > RAM

  /* The .bss section needs to be zeroed out, so it's similar to
   * .data in that we need to remember start and end and make it easy
   * to zero in int32 sizes instead of byte per byte. We however
   * don't need to put this in flash */
  .bss : {
    . = ALIGN(4);
    _sbss = .;
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
  } > RAM

  /* Create a symbol for the end of the RAM as we will put the stack there */
  _eram = ORIGIN(RAM) + LENGTH(RAM);
}
