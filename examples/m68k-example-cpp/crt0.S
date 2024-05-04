#include "asm.h"
#include "mc68332.inc"

  .title "crt0.S"

  .extern SYM(main)
  .extern __stack
  .extern __bss_start
  .extern __text_start
  .extern SYM(init_main)

  .align 2
  .section  .text

__start:
  /* disable all interrupts on startup. */
  nop
  move.w  #0x2700, sr

setup_system_module:
  /* enable supervisor and map the register block to 0xfff000 */
  move.w #0x604f,SIMCR

setup_system_protection:
  /* enable external bus monitor */
  move.b #0x0,SYPCR

setup_clock_module:
  /* set system clock to 16.777 MHz */
  move.w #0x7f00,SYNCR

setup_cs_pins:
  /* CSBOOT, CS0-CS5 to 16 bit */
  move.w #0x3fff,CSPAR0
  /* CS10, CS9, CS8 --> 8 Bit, CS6 --> ADDR19 */
  move.w #0x02ad,CSPAR1

setup_cs_ram:
  /* CS0 to 0x100000 with 1M even byte */
  move.w #0x1007,CSBAR0
  move.w #0x5830,CSOR0
  /* CS1 to 0x100000 with 1M odd byte */
  move.w #0x1007,CSBAR1
  move.w #0x3830,CSOR1
  /* CS2 to 0x200000 with 64k even byte */
  move.w #0x2003,CSBAR2
  move.w #0x5830,CSOR2
  /* CS3 to 0x200000 with 64k odd byte */
  move.w #0x2003,CSBAR3
  move.w #0x3830,CSOR3

  /* prepare stack and frame pointer. */
  move.l  #__stack, sp
  link    a6, #-8

  /* copy vectors to ram */
  jsr SYM(copy_vec)

  /* copy data to ram */
  jsr SYM(copy_data)

  /* clear bss */
  jsr SYM(clear_bss)

  /* initialize hardware. */
  jsr     SYM(init_main)

  /* enable interrupts. */
  move.w  #0x2000, sr

  /* call main(int argc, char *argv[] */
  move.l  #0, -(sp)
  move.l  #0, -(sp)
  move.l  #0, -(sp)
  jsr     SYM(main)
  lea     (sp, 12), sp

  /* stop on exit from main. */
__exit:
  bra __exit
