/**
 * <H3>
 * Projekt: --
 * </H3>
 *
 * @file uart.h
 * @author Frank Kunz
 * @date 28.10.2008
 *
 * Declarations for MC68332 UART driver.
 *
 * $Header: /home/cvsrep/mx4000/drv/uart.h,v 1.2 2008/11/30 19:11:05 frank Exp $
 * ------------------------------------
 * $Log: uart.h,v $
 * Revision 1.2  2008/11/30 19:11:05  frank
 * interrupt driven rx added
 *
 * Revision 1.1.1.1  2008/11/16 14:43:15  frank
 * Added
 *
 *
 * ------------------------------------
 */
/** UART_H */
#ifndef UART_H
#define UART_H

/* --- Includes --- */

/* --- Defines --- */

/* --- Typedefs --- */

/* --- extern --- */
extern void v_uartInit(void);
extern int i_uartGetch(void);
extern void v_uartPutch(unsigned int ch);
extern void uartIsr(void);

#endif
