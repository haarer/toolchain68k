/*************************************************************************
Title:    Interrupt UART library with receive/transmit circular buffers
Author:   Peter Fleury <pfleury@gmx.ch>   http://tinyurl.com/peterfleury
File:     $Id: uart.c,v 1.15.2.4 2015/09/05 18:33:32 peter Exp $
Software: AVR-GCC 4.x
Hardware: any AVR with built-in UART, 
License:  GNU General Public License 
          
DESCRIPTION:
    An interrupt is generated when the UART has finished transmitting or
    receiving a byte. The interrupt handling routines use circular buffers
    for buffering received and transmitted data.
    
    The UART_RX_BUFFER_SIZE and UART_TX_BUFFER_SIZE variables define
    the buffer size in bytes. Note that these variables must be a 
    power of 2.
    
USAGE:
    Refere to the header file uart.h for a description of the routines. 
    See also example test_uart.c.

NOTES:
    Based on Atmel Application Note AVR306
                    
LICENSE:
    Copyright (C) 2015 Peter Fleury, GNU General Public License Version 3

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
                        
*************************************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "uart.h"


/*
 *  constants and macros
 */

/* size of RX/TX buffers */
#define UART_RX_BUFFER_MASK ( UART_RX_BUFFER_SIZE - 1)
#define UART_TX_BUFFER_MASK ( UART_TX_BUFFER_SIZE - 1)

#if ( UART_RX_BUFFER_SIZE & UART_RX_BUFFER_MASK )
#error RX buffer size is not a power of 2
#endif
#if ( UART_TX_BUFFER_SIZE & UART_TX_BUFFER_MASK )
#error TX buffer size is not a power of 2
#endif


#if defined(__AVR_AT90S2313__) || defined(__AVR_AT90S4414__) || defined(__AVR_AT90S8515__) || \
    defined(__AVR_AT90S4434__) || defined(__AVR_AT90S8535__) || \
    defined(__AVR_ATmega103__)
 /* old AVR classic or ATmega103 with one UART */
 #define UART0_RECEIVE_INTERRUPT   UART_RX_vect 
 #define UART0_TRANSMIT_INTERRUPT  UART_UDRE_vect
 #define UART0_STATUS      USR
 #define UART0_CONTROL     UCR
 #define UART0_DATA        UDR  
 #define UART0_UDRIE       UDRIE
 #define UART0_UBRRL       UBRR
 #define UART0_BIT_U2X     U2X
 #define UART0_BIT_RXCIE   RXCIE
 #define UART0_BIT_RXEN    RXEN
 #define UART0_BIT_TXEN    TXEN
#elif defined(__AVR_AT90S2333__) || defined(__AVR_AT90S4433__)
 /* old AVR classic with one UART */
 #define UART0_RECEIVE_INTERRUPT   UART_RX_vect 
 #define UART0_TRANSMIT_INTERRUPT  UART_UDRE_vect
 #define UART0_STATUS      UCSRA
 #define UART0_CONTROL     UCSRB
 #define UART0_DATA        UDR 
 #define UART0_UDRIE       UDRIE
 #define UART0_UBRRL       UBRR
 #define UART0_BIT_U2X     U2X
 #define UART0_BIT_RXCIE   RXCIE
 #define UART0_BIT_RXEN    RXEN
 #define UART0_BIT_TXEN    TXEN
#elif defined(__AVR_AT90PWM216__) || defined(__AVR_AT90PWM316__) 
 /* AT90PWN216/316 with one USART */
 #define UART0_RECEIVE_INTERRUPT   USART_RX_vect
 #define UART0_TRANSMIT_INTERRUPT  USART_UDRE_vect
 #define UART0_STATUS      UCSRA
 #define UART0_CONTROL     UCSRB
 #define UART0_CONTROLC    UCSRC
 #define UART0_DATA        UDR
 #define UART0_UDRIE       UDRIE
 #define UART0_UBRRL       UBRRL
 #define UART0_UBRRH       UBRRH
 #define UART0_BIT_U2X     U2X
 #define UART0_BIT_RXCIE   RXCIE
 #define UART0_BIT_RXEN    RXEN
 #define UART0_BIT_TXEN    TXEN
 #define UART0_BIT_UCSZ0   UCSZ0
 #define UART0_BIT_UCSZ1   UCSZ1 
#elif defined(__AVR_ATmega8__) || defined(__AVR_ATmega8A__) || \
      defined(__AVR_ATmega16__) || defined(__AVR_ATmega16A__) || \
      defined(__AVR_ATmega32__) || defined(__AVR_ATmega32A__) || \
      defined(__AVR_ATmega323__)
 /* ATmega with one USART */
 #define UART0_RECEIVE_INTERRUPT   USART_RXC_vect
 #define UART0_TRANSMIT_INTERRUPT  USART_UDRE_vect
 #define UART0_STATUS      UCSRA
 #define UART0_CONTROL     UCSRB
 #define UART0_CONTROLC    UCSRC
 #define UART0_DATA        UDR
 #define UART0_UDRIE       UDRIE
 #define UART0_UBRRL       UBRRL
 #define UART0_UBRRH       UBRRH
 #define UART0_BIT_U2X     U2X
 #define UART0_BIT_RXCIE   RXCIE
 #define UART0_BIT_RXEN    RXEN
 #define UART0_BIT_TXEN    TXEN
 #define UART0_BIT_UCSZ0   UCSZ0
 #define UART0_BIT_UCSZ1   UCSZ1
 #define UART0_BIT_URSEL   URSEL
#elif defined (__AVR_ATmega8515__) || defined(__AVR_ATmega8535__)
 #define UART0_RECEIVE_INTERRUPT   USART_RX_vect
 #define UART0_TRANSMIT_INTERRUPT  USART_UDRE_vect
 #define UART0_STATUS      UCSRA
 #define UART0_CONTROL     UCSRB
 #define UART0_CONTROLC    UCSRC 
 #define UART0_DATA        UDR
 #define UART0_UDRIE       UDRIE
 #define UART0_UBRRL       UBRRL
 #define UART0_UBRRH       UBRRH
 #define UART0_BIT_U2X     U2X
 #define UART0_BIT_RXCIE   RXCIE
 #define UART0_BIT_RXEN    RXEN
 #define UART0_BIT_TXEN    TXEN
 #define UART0_BIT_UCSZ0   UCSZ0
 #define UART0_BIT_UCSZ1   UCSZ1
 #define UART0_BIT_URSEL   URSEL
#elif defined(__AVR_ATmega163__)
  /* ATmega163 with one UART */
 #define UART0_RECEIVE_INTERRUPT   UART_RX_vect
 #define UART0_TRANSMIT_INTERRUPT  UART_UDRE_vect
 #define UART0_STATUS      UCSRA
 #define UART0_CONTROL     UCSRB
 #define UART0_DATA        UDR
 #define UART0_UDRIE       UDRIE
 #define UART0_UBRRL       UBRR
 #define UART0_UBRRH       UBRRHI
 #define UART0_BIT_U2X     U2X
 #define UART0_BIT_RXCIE   RXCIE
 #define UART0_BIT_RXEN    RXEN
 #define UART0_BIT_TXEN    TXEN
#elif defined(__AVR_ATmega162__) 
 /* ATmega with two USART */
 #define ATMEGA_USART1
 #define UART0_RECEIVE_INTERRUPT   USART0_RXC_vect
 #define UART1_RECEIVE_INTERRUPT   USART1_RXC_vect
 #define UART0_TRANSMIT_INTERRUPT  USART0_UDRE_vect
 #define UART1_TRANSMIT_INTERRUPT  USART1_UDRE_vect
 #define UART0_STATUS      UCSR0A
 #define UART0_CONTROL     UCSR0B
 #define UART0_CONTROLC    UCSR0C
 #define UART0_DATA        UDR0
 #define UART0_UDRIE       UDRIE0
 #define UART0_UBRRL       UBRR0L
 #define UART0_UBRRH       UBRR0H
 #define UART0_BIT_URSEL   URSEL0
 #define UART0_BIT_U2X     U2X0
 #define UART0_BIT_RXCIE   RXCIE0
 #define UART0_BIT_RXEN    RXEN0
 #define UART0_BIT_TXEN    TXEN0
 #define UART0_BIT_UCSZ0   UCSZ00
 #define UART0_BIT_UCSZ1   UCSZ01
 #define UART1_STATUS      UCSR1A
 #define UART1_CONTROL     UCSR1B
 #define UART1_CONTROLC    UCSR1C
 #define UART1_DATA        UDR1
 #define UART1_UDRIE       UDRIE1
 #define UART1_UBRRL       UBRR1L
 #define UART1_UBRRH       UBRR1H
 #define UART1_BIT_URSEL   URSEL1
 #define UART1_BIT_U2X     U2X1
 #define UART1_BIT_RXCIE   RXCIE1
 #define UART1_BIT_RXEN    RXEN1
 #define UART1_BIT_TXEN    TXEN1
 #define UART1_BIT_UCSZ0   UCSZ10
 #define UART1_BIT_UCSZ1   UCSZ11 
#elif defined(__AVR_ATmega161__)
 /* ATmega with UART */
 #error "AVR ATmega161 currently not supported by this libaray !"
#elif defined(__AVR_ATmega169__)
 /* ATmega with one USART */
 #define UART0_RECEIVE_INTERRUPT   USART0_RX_vect
 #define UART0_TRANSMIT_INTERRUPT  USART0_UDRE_vect
 #define UART0_STATUS      UCSRA
 #define UART0_CONTROL     UCSRB
 #define UART0_CONTROLC    UCSRC
 #define UART0_DATA        UDR
 #define UART0_UDRIE       UDRIE
 #define UART0_UBRRL       UBRRL
 #define UART0_UBRRH       UBRRH
 #define UART0_BIT_U2X     U2X
 #define UART0_BIT_RXCIE   RXCIE
 #define UART0_BIT_RXEN    RXEN
 #define UART0_BIT_TXEN    TXEN
 #define UART0_BIT_UCSZ0   UCSZ0
 #define UART0_BIT_UCSZ1   UCSZ1
#elif defined(__AVR_ATmega48__) || defined(__AVR_ATmega48A__) || defined(__AVR_ATmega48P__) || defined(__AVR_ATmega48PA__) || defined(__AVR_ATmega48PB__) || \
      defined(__AVR_ATmega88__) || defined(__AVR_ATmega88A__) || defined(__AVR_ATmega88P__) || defined(__AVR_ATmega88PA__) || defined(__AVR_ATmega88PB__) || \
      defined(__AVR_ATmega168__) || defined(__AVR_ATmega168A__)|| defined(__AVR_ATmega168P__)|| defined(__AVR_ATmega168PA__) || defined(__AVR_ATmega168PB__) || \
      defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) || \
      defined(__AVR_ATmega3250__) || defined(__AVR_ATmega3290__) ||defined(__AVR_ATmega6450__) || defined(__AVR_ATmega6490__)
 /* ATmega with one USART */
 #define UART0_RECEIVE_INTERRUPT   USART_RX_vect
 #define UART0_TRANSMIT_INTERRUPT  USART_UDRE_vect
 #define UART0_STATUS      UCSR0A
 #define UART0_CONTROL     UCSR0B
 #define UART0_CONTROLC    UCSR0C
 #define UART0_DATA        UDR0
 #define UART0_UDRIE       UDRIE0
 #define UART0_UBRRL       UBRR0L
 #define UART0_UBRRH       UBRR0H
 #define UART0_BIT_U2X     U2X0
 #define UART0_BIT_RXCIE   RXCIE0
 #define UART0_BIT_RXEN    RXEN0
 #define UART0_BIT_TXEN    TXEN0
 #define UART0_BIT_UCSZ0   UCSZ00
 #define UART0_BIT_UCSZ1   UCSZ01
#elif defined(__AVR_ATtiny2313__) || defined(__AVR_ATtiny2313A__) || defined(__AVR_ATtiny4313__)
 /* ATtiny with one USART */
 #define UART0_RECEIVE_INTERRUPT   USART_RX_vect
 #define UART0_TRANSMIT_INTERRUPT  USART_UDRE_vect
 #define UART0_STATUS      UCSRA
 #define UART0_CONTROL     UCSRB
 #define UART0_CONTROLC    UCSRC
 #define UART0_DATA        UDR
 #define UART0_UDRIE       UDRIE
 #define UART0_UBRRL       UBRRL
 #define UART0_UBRRH       UBRRH
 #define UART0_BIT_U2X     U2X
 #define UART0_BIT_RXCIE   RXCIE
 #define UART0_BIT_RXEN    RXEN
 #define UART0_BIT_TXEN    TXEN
 #define UART0_BIT_UCSZ0   UCSZ0
 #define UART0_BIT_UCSZ1   UCSZ1
#elif defined(__AVR_ATmega329__) || defined(__AVR_ATmega649__) || defined(__AVR_ATmega3290__) || defined(__AVR_ATmega6490__) ||\
      defined(__AVR_ATmega169A__) || defined(__AVR_ATmega169PA__) || \
      defined(__AVR_ATmega329A__) || defined(__AVR_ATmega329PA__) || defined(__AVR_ATmega3290A__) || defined(__AVR_ATmega3290PA__) || \
      defined(__AVR_ATmega649A__) || defined(__AVR_ATmega649P__) || defined(__AVR_ATmega6490A__) || defined(__AVR_ATmega6490P__) || \
      defined(__AVR_ATmega165__) || defined(__AVR_ATmega325__) || defined(__AVR_ATmega645__) || defined(__AVR_ATmega3250__) || defined(__AVR_ATmega6450__) || \
      defined(__AVR_ATmega165A__) || defined(__AVR_ATmega165PA__) || \
      defined(__AVR_ATmega325A__) || defined(__AVR_ATmega325PA__) || defined(__AVR_ATmega3250A__) || defined(__AVR_ATmega3250PA__) ||\
      defined(__AVR_ATmega645A__) || defined(__AVR_ATmega645PA__) || defined(__AVR_ATmega6450A__) || defined(__AVR_ATmega6450PA__) || \
      defined(__AVR_ATmega644__)
 /* ATmega with one USART */
 #define UART0_RECEIVE_INTERRUPT   USART0_RX_vect
 #define UART0_TRANSMIT_INTERRUPT  USART0_UDRE_vect
 #define UART0_STATUS      UCSR0A
 #define UART0_CONTROL     UCSR0B
 #define UART0_CONTROLC    UCSR0C
 #define UART0_DATA        UDR0
 #define UART0_UDRIE       UDRIE0
 #define UART0_UBRRL       UBRR0L
 #define UART0_UBRRH       UBRR0H
 #define UART0_BIT_U2X     U2X0
 #define UART0_BIT_RXCIE   RXCIE0
 #define UART0_BIT_RXEN    RXEN0
 #define UART0_BIT_TXEN    TXEN0
 #define UART0_BIT_UCSZ0   UCSZ00
 #define UART0_BIT_UCSZ1   UCSZ01
#elif defined(__AVR_ATmega64__) || defined(__AVR_ATmega128__) || defined(__AVR_ATmega128A__) ||\
      defined(__AVR_ATmega640__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__) || \
      defined(__AVR_ATmega164P__) || defined(__AVR_ATmega324P__) || defined(__AVR_ATmega644P__) ||  \
      defined(__AVR_ATmega164A__) || defined(__AVR_ATmega164PA__) || defined(__AVR_ATmega324A__) || defined(__AVR_ATmega324PA__) || \
      defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644PA__) || defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) ||\
      defined(__AVR_ATtiny1634__)
 /* ATmega with two USART */
 #define ATMEGA_USART1
 #define UART0_RECEIVE_INTERRUPT   USART0_RX_vect
 #define UART1_RECEIVE_INTERRUPT   USART1_RX_vect
 #define UART0_TRANSMIT_INTERRUPT  USART0_UDRE_vect
 #define UART1_TRANSMIT_INTERRUPT  USART1_UDRE_vect
 #define UART0_STATUS      UCSR0A
 #define UART0_CONTROL     UCSR0B
 #define UART0_CONTROLC    UCSR0C  
 #define UART0_DATA        UDR0
 #define UART0_UDRIE       UDRIE0
 #define UART0_UBRRL       UBRR0L
 #define UART0_UBRRH       UBRR0H
 #define UART0_BIT_U2X     U2X0
 #define UART0_BIT_RXCIE   RXCIE0
 #define UART0_BIT_RXEN    RXEN0
 #define UART0_BIT_TXEN    TXEN0
 #define UART0_BIT_UCSZ0   UCSZ00
 #define UART0_BIT_UCSZ1   UCSZ01 
 #define UART1_STATUS      UCSR1A
 #define UART1_CONTROL     UCSR1B
 #define UART1_CONTROLC    UCSR1C  
 #define UART1_DATA        UDR1
 #define UART1_UDRIE       UDRIE1
 #define UART1_UBRRL       UBRR1L
 #define UART1_UBRRH       UBRR1H
 #define UART1_BIT_U2X     U2X1
 #define UART1_BIT_RXCIE   RXCIE1
 #define UART1_BIT_RXEN    RXEN1
 #define UART1_BIT_TXEN    TXEN1
 #define UART1_BIT_UCSZ0   UCSZ10
 #define UART1_BIT_UCSZ1   UCSZ11
#elif defined(__AVR_ATmega8U2__) || defined(__AVR_ATmega16U2__) || defined(__AVR_ATmega32U2__) || \
      defined(__AVR_ATmega16U4__) || defined(__AVR_ATmega32U4__) || \
      defined(__AVR_AT90USB82__) || defined(__AVR_AT90USB162__) || \
      defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB647__) || defined(__AVR_AT90USB1287__)
 #define UART0_RECEIVE_INTERRUPT   USART1_RX_vect
 #define UART0_TRANSMIT_INTERRUPT  USART1_UDRE_vect
 #define UART0_STATUS      UCSR1A
 #define UART0_CONTROL     UCSR1B
 #define UART0_CONTROLC    UCSR1C
 #define UART0_DATA        UDR1
 #define UART0_UDRIE       UDRIE1
 #define UART0_UBRRL       UBRR1L
 #define UART0_UBRRH       UBRR1H
 #define UART0_BIT_U2X     U2X1
 #define UART0_BIT_RXCIE   RXCIE1
 #define UART0_BIT_RXEN    RXEN1
 #define UART0_BIT_TXEN    TXEN1
 #define UART0_BIT_UCSZ0   UCSZ10
 #define UART0_BIT_UCSZ1   UCSZ11
#else
 #error "no UART definition for MCU available"
#endif



/*
 *  module global variables
 */
static volatile unsigned char UART_TxBuf[UART_TX_BUFFER_SIZE];
static volatile unsigned char UART_RxBuf[UART_RX_BUFFER_SIZE];
static volatile unsigned char UART_TxHead;
static volatile unsigned char UART_TxTail;
static volatile unsigned char UART_RxHead;
static volatile unsigned char UART_RxTail;
static volatile unsigned char UART_LastRxError;

#if defined( ATMEGA_USART1 )
static volatile unsigned char UART1_TxBuf[UART_TX_BUFFER_SIZE];
static volatile unsigned char UART1_RxBuf[UART_RX_BUFFER_SIZE];
static volatile unsigned char UART1_TxHead;
static volatile unsigned char UART1_TxTail;
static volatile unsigned char UART1_RxHead;
static volatile unsigned char UART1_RxTail;
static volatile unsigned char UART1_LastRxError;
#endif



ISR (UART0_RECEIVE_INTERRUPT)	
/*************************************************************************
Function: UART Receive Complete interrupt
Purpose:  called when the UART has received a character
**************************************************************************/
{
    unsigned char tmphead;
    unsigned char data;
    unsigned char usr;
    unsigned char lastRxError;
 
 
    /* read UART status register and UART data register */
    usr  = UART0_STATUS;
    data = UART0_DATA;
    
    /* get FEn (Frame Error) DORn (Data OverRun) UPEn (USART Parity Error) bits */
#if defined(FE) && defined(DOR) && defined(UPE)
    lastRxError = usr & (_BV(FE)|_BV(DOR)|_BV(UPE) );
#elif defined(FE0) && defined(DOR0) && defined(UPE0)
    lastRxError = usr & (_BV(FE0)|_BV(DOR0)|_BV(UPE0) );
#elif defined(FE1) && defined(DOR1) && defined(UPE1)
    lastRxError = usr & (_BV(FE1)|_BV(DOR1)|_BV(UPE1) );
#elif defined(FE) && defined(DOR)
    lastRxError = usr & (_BV(FE)|_BV(DOR) );
#endif

    /* calculate buffer index */ 
    tmphead = ( UART_RxHead + 1) & UART_RX_BUFFER_MASK;
    
    if ( tmphead == UART_RxTail ) {
        /* error: receive buffer overflow */
        lastRxError = UART_BUFFER_OVERFLOW >> 8;
    }else{
        /* store new index */
        UART_RxHead = tmphead;
        /* store received data in buffer */
        UART_RxBuf[tmphead] = data;
    }
    UART_LastRxError |= lastRxError;   
}


ISR (UART0_TRANSMIT_INTERRUPT)
/*************************************************************************
Function: UART Data Register Empty interrupt
Purpose:  called when the UART is ready to transmit the next byte
**************************************************************************/
{
    unsigned char tmptail;

    
    if ( UART_TxHead != UART_TxTail) {
        /* calculate and store new buffer index */
        tmptail = (UART_TxTail + 1) & UART_TX_BUFFER_MASK;
        UART_TxTail = tmptail;
        /* get one byte from buffer and write it to UART */
        UART0_DATA = UART_TxBuf[tmptail];  /* start transmission */
    }else{
        /* tx buffer empty, disable UDRE interrupt */
        UART0_CONTROL &= ~_BV(UART0_UDRIE);
    }
}


/*************************************************************************
Function: uart_init()
Purpose:  initialize UART and set baudrate
Input:    baudrate using macro UART_BAUD_SELECT()
Returns:  none
**************************************************************************/
void uart_init(unsigned int baudrate)
{
    UART_TxHead = 0;
    UART_TxTail = 0;
    UART_RxHead = 0;
    UART_RxTail = 0;

#ifdef UART_TEST
#ifndef UART0_BIT_U2X
#warning "UART0_BIT_U2X not defined"
#endif
#ifndef UART0_UBRRH
#warning "UART0_UBRRH not defined"
#endif
#ifndef UART0_CONTROLC
#warning "UART0_CONTROLC not defined"
#endif
#if defined(URSEL) || defined(URSEL0)
#ifndef UART0_BIT_URSEL
#warning "UART0_BIT_URSEL not defined"
#endif
#endif
#endif

    /* Set baud rate */
    if ( baudrate & 0x8000 )
    {
        #if UART0_BIT_U2X
        UART0_STATUS = (1<<UART0_BIT_U2X);  //Enable 2x speed 
        #endif
    } 
    #if defined(UART0_UBRRH)
    UART0_UBRRH = (unsigned char)((baudrate>>8)&0x80) ;
    #endif    
    UART0_UBRRL = (unsigned char) (baudrate&0x00FF);
      
    /* Enable USART receiver and transmitter and receive complete interrupt */
    UART0_CONTROL = _BV(UART0_BIT_RXCIE)|(1<<UART0_BIT_RXEN)|(1<<UART0_BIT_TXEN);
    
    /* Set frame format: asynchronous, 8data, no parity, 1stop bit */
    #ifdef UART0_CONTROLC
    #ifdef UART0_BIT_URSEL
    UART0_CONTROLC = (1<<UART0_BIT_URSEL)|(1<<UART0_BIT_UCSZ1)|(1<<UART0_BIT_UCSZ0);
    #else
    UART0_CONTROLC = (1<<UART0_BIT_UCSZ1)|(1<<UART0_BIT_UCSZ0);
    #endif 
    #endif

}/* uart_init */


/*************************************************************************
Function: uart_getc()
Purpose:  return byte from ringbuffer  
Returns:  lower byte:  received byte from ringbuffer
          higher byte: last receive error
**************************************************************************/
unsigned int uart_getc(void)
{    
    unsigned char tmptail;
    unsigned char data;
    unsigned char lastRxError;


    if ( UART_RxHead == UART_RxTail ) {
        return UART_NO_DATA;   /* no data available */
    }
    
    /* calculate buffer index */
    tmptail = (UART_RxTail + 1) & UART_RX_BUFFER_MASK;
    
    /* get data from receive buffer */
    data = UART_RxBuf[tmptail];
    lastRxError = UART_LastRxError;
    
    /* store buffer index */
    UART_RxTail = tmptail; 
    
    UART_LastRxError = 0;
    return (lastRxError << 8) + data;

}/* uart_getc */


/*************************************************************************
Function: uart_putc()
Purpose:  write byte to ringbuffer for transmitting via UART
Input:    byte to be transmitted
Returns:  none          
**************************************************************************/
void uart_putc(unsigned char data)
{
    unsigned char tmphead;

    
    tmphead  = (UART_TxHead + 1) & UART_TX_BUFFER_MASK;
    
    while ( tmphead == UART_TxTail ){
        ;/* wait for free space in buffer */
    }
    
    UART_TxBuf[tmphead] = data;
    UART_TxHead = tmphead;

    /* enable UDRE interrupt */
    UART0_CONTROL    |= _BV(UART0_UDRIE);

}/* uart_putc */


/*************************************************************************
Function: uart_puts()
Purpose:  transmit string to UART
Input:    string to be transmitted
Returns:  none          
**************************************************************************/
void uart_puts(const char *s )
{
    while (*s) 
      uart_putc(*s++);

}/* uart_puts */


/*************************************************************************
Function: uart_puts_p()
Purpose:  transmit string from program memory to UART
Input:    program memory string to be transmitted
Returns:  none
**************************************************************************/
void uart_puts_p(const char *progmem_s )
{
    register char c;
    
    while ( (c = pgm_read_byte(progmem_s++)) ) 
      uart_putc(c);

}/* uart_puts_p */


/*
 * these functions are only for ATmegas with two USART
 */
#if defined( ATMEGA_USART1 )

ISR(UART1_RECEIVE_INTERRUPT)
/*************************************************************************
Function: UART1 Receive Complete interrupt
Purpose:  called when the UART1 has received a character
**************************************************************************/
{
    unsigned char tmphead;
    unsigned char data;
    unsigned char usr;
    unsigned char lastRxError;
 
 
    /* read UART status register and UART data register */ 
    usr  = UART1_STATUS;
    data = UART1_DATA;
    
    /* get FEn (Frame Error) DORn (Data OverRun) UPEn (USART Parity Error) bits */
    lastRxError = usr & (_BV(FE1)|_BV(DOR1)|_BV(UPE1) );
            
    /* calculate buffer index */ 
    tmphead = ( UART1_RxHead + 1) & UART_RX_BUFFER_MASK;
    
    if ( tmphead == UART1_RxTail ) {
        /* error: receive buffer overflow */
        lastRxError = UART_BUFFER_OVERFLOW >> 8;
    }else{
        /* store new index */
        UART1_RxHead = tmphead;
        /* store received data in buffer */
        UART1_RxBuf[tmphead] = data;
    }
    UART1_LastRxError |= lastRxError;   
}


ISR(UART1_TRANSMIT_INTERRUPT)
/*************************************************************************
Function: UART1 Data Register Empty interrupt
Purpose:  called when the UART1 is ready to transmit the next byte
**************************************************************************/
{
    unsigned char tmptail;

    
    if ( UART1_TxHead != UART1_TxTail) {
        /* calculate and store new buffer index */
        tmptail = (UART1_TxTail + 1) & UART_TX_BUFFER_MASK;
        UART1_TxTail = tmptail;
        /* get one byte from buffer and write it to UART */
        UART1_DATA = UART1_TxBuf[tmptail];  /* start transmission */
    }else{
        /* tx buffer empty, disable UDRE interrupt */
        UART1_CONTROL &= ~_BV(UART1_UDRIE);
    }
}


/*************************************************************************
Function: uart1_init()
Purpose:  initialize UART1 and set baudrate
Input:    baudrate using macro UART_BAUD_SELECT()
Returns:  none
**************************************************************************/
void uart1_init(unsigned int baudrate)
{
    UART1_TxHead = 0;
    UART1_TxTail = 0;
    UART1_RxHead = 0;
    UART1_RxTail = 0;

#ifdef UART_TEST
#ifndef UART1_BIT_U2X
#warning "UART1_BIT_U2X not defined"
#endif
#ifndef UART1_UBRRH
#warning "UART1_UBRRH not defined"
#endif
#ifndef UART1_CONTROLC
#warning "UART1_CONTROLC not defined"
#endif
#if defined(URSEL) || defined(URSEL1)
#ifndef UART1_BIT_URSEL
#warning "UART1_BIT_URSEL not defined"
#endif
#endif
#endif

    /* Set baud rate */
    if ( baudrate & 0x8000 ) 
    {
        #if UART1_BIT_U2X
    	UART1_STATUS = (1<<UART1_BIT_U2X);  //Enable 2x speed 
        #endif
    }
    UART1_UBRRH = (unsigned char)((baudrate>>8)&0x80) ;
    UART1_UBRRL = (unsigned char) baudrate;
        
    /* Enable USART receiver and transmitter and receive complete interrupt */
    UART1_CONTROL = _BV(UART1_BIT_RXCIE)|(1<<UART1_BIT_RXEN)|(1<<UART1_BIT_TXEN);    
    
    /* Set frame format: asynchronous, 8data, no parity, 1stop bit */   
    #ifdef UART1_BIT_URSEL
    UART1_CONTROLC = (1<<UART1_BIT_URSEL)|(1<<UART1_BIT_UCSZ1)|(1<<UART1_BIT_UCSZ0);
    #else
    UART1_CONTROLC = (1<<UART1_BIT_UCSZ1)|(1<<UART1_BIT_UCSZ0);
    #endif 
    
}/* uart_init */


/*************************************************************************
Function: uart1_getc()
Purpose:  return byte from ringbuffer  
Returns:  lower byte:  received byte from ringbuffer
          higher byte: last receive error
**************************************************************************/
unsigned int uart1_getc(void)
{    
    unsigned char tmptail;
    unsigned int  data;
    unsigned char lastRxError;


    if ( UART1_RxHead == UART1_RxTail ) {
        return UART_NO_DATA;   /* no data available */
    }
    
    /* calculate buffer index */
    tmptail = (UART1_RxTail + 1) & UART_RX_BUFFER_MASK;
    
    /* get data from receive buffer */
    data = UART1_RxBuf[tmptail];
    lastRxError = UART1_LastRxError;
    
    /* store buffer index */
    UART1_RxTail = tmptail; 
    
    UART1_LastRxError = 0;
    return (lastRxError << 8) + data;

}/* uart1_getc */


/*************************************************************************
Function: uart1_putc()
Purpose:  write byte to ringbuffer for transmitting via UART
Input:    byte to be transmitted
Returns:  none          
**************************************************************************/
void uart1_putc(unsigned char data)
{
    unsigned char tmphead;

    
    tmphead  = (UART1_TxHead + 1) & UART_TX_BUFFER_MASK;
    
    while ( tmphead == UART1_TxTail ){
        ;/* wait for free space in buffer */
    }
    
    UART1_TxBuf[tmphead] = data;
    UART1_TxHead = tmphead;

    /* enable UDRE interrupt */
    UART1_CONTROL    |= _BV(UART1_UDRIE);

}/* uart1_putc */


/*************************************************************************
Function: uart1_puts()
Purpose:  transmit string to UART1
Input:    string to be transmitted
Returns:  none          
**************************************************************************/
void uart1_puts(const char *s )
{
    while (*s) 
      uart1_putc(*s++);

}/* uart1_puts */


/*************************************************************************
Function: uart1_puts_p()
Purpose:  transmit string from program memory to UART1
Input:    program memory string to be transmitted
Returns:  none
**************************************************************************/
void uart1_puts_p(const char *progmem_s )
{
    register char c;
    
    while ( (c = pgm_read_byte(progmem_s++)) ) 
      uart1_putc(c);

}/* uart1_puts_p */


#endif
