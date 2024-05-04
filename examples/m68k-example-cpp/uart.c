/**
 * <H3>
 * Projekt: --
 * </H3>
 *
 * @file uart.c
 * @author Frank Kunz
 * @date 28.10.2008
 *
 * MC68332 UART driver.
 *
 * ------------------------------------
 * $Log: uart.c,v $
 * Revision 1.3  2009/12/13 19:02:50  frank
 * enhanced RX for non blocking reads
 *
 * Revision 1.2  2008/11/30 19:11:05  frank
 * interrupt driven rx added
 *
 * Revision 1.1.1.1  2008/11/16 14:43:15  frank
 * Added
 *
 *
 * ------------------------------------
 */

/* --- Includes --- */
#include <uart.h>
#include <mc68332.h>
#include <stdio.h>

/* --- Defines --- */

/* --- Typedefs --- */

/* --- extern --- */

/* --- local --- */

/* --- Prototypes --- */

/*
 * FUNCTIONS
 */

void v_uartInit(void)
{
//  SCCR0 = 0x0037; // 9600 Baud
  SCCR0 = 0x001B; // 19200 Baud
  SCCR1 = 0x000C; // 10 bit, no parity, TX and RX enabled
}

int i_uartGetch(void)
{
  while(!(SCSR & (1<<6)));  // check for receiver has data
  return(SCDR);
}

void v_uartPutch(unsigned int ch)
{
  while(!(SCSR & (1<<8)));  // wait for transmitter ready
  SCDR = ch;
}
