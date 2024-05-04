#include <stdio.h>
#include "mc68332.h"
#include <string.h>
#include <reent.h>
#include "uart.h"

struct _reent reent_main;

void            init_main( void );
static void     init_basics( void );
static void     init_clock_config( void );
static void     init_peripherials( void );
static void     init_chip_selects( void );

void
init_main( void )
{
  init_basics(  );
  init_clock_config(  );
  init_peripherials(  );
  v_uartInit();

  /* Initialise individual modules */
  init_chip_selects(  );

  _REENT_INIT_PTR(&reent_main);
  // why is this not visible ? 
  __sinit(&reent_main);
  _impure_ptr = &reent_main;
}

static void
init_basics( void )
{
}

static void
init_clock_config( void )
{
  /*
   * Clock module uses normal PLL mode with 32.768 kHz external reference (Fref)
   *  W = 0, X = 1, Y = 63
   *   System clock frequency = 16.777 MHz
   *   Loss of clock detection disabled
   *   Reset/Interrupt on loss of lock disabled
   */
  SYNCR = 0x3F00;       /* Set W and Y */
  while( ( SYNCR & 0x08 ) == 0 );    /* Wait for PLL to lock */
  SYNCR = 0x7F00;       /* Set desired X */
}

static void
init_chip_selects( void )
{
  // assign external interrupt pins to Port
  PFPAR = 0;

}

static void
init_peripherials( void )
{
}

void copy_vec(void)
{
  extern int __rom_vec_start;
  extern int __ram_vec_start;
  extern int __ram_vec_end;
  extern int __vec_start;
  int *rom_vec_start = &__rom_vec_start;
  int *ram_vec_start = &__ram_vec_start;
  int *ram_vec_end = &__ram_vec_end;

  if(ram_vec_start != ram_vec_end)
  {
    bcopy(rom_vec_start, ram_vec_start, (ram_vec_end - ram_vec_start) * sizeof(int));

    /* Exception vector table at vec_start */
    asm volatile ( "move.l   %0,%%d0": :"i"( &__ram_vec_start ) );
    asm volatile ( "movec    %d0,%vbr" );
  }
  else
  {
    /* Exception vector table at vec_start */
    asm volatile ( "move.l   %0,%%d0": :"i"( &__vec_start ) );
    asm volatile ( "movec    %d0,%vbr" );
  }
}

void clear_bss(void)
{
  extern int __bss_start;
  extern int __bss_end;
  int *bss_start = &__bss_start;
  int *bss_end = &__bss_end;
  bzero(bss_start, (bss_end - bss_start) * sizeof(int));
}

void copy_data(void)
{
  extern int __end_text;
  extern int __data_start;
  extern int __data_end;
  int *end_text = &__end_text;
  int *data_start = &__data_start;
  int *data_end = &__data_end;
  if(end_text != data_start)
    bcopy(end_text, data_start, (data_end - data_start) * sizeof(int));
}
