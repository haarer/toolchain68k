
// 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>




// allows to attach uart to stdio 

#include <stdio.h>

#include <avr/io.h>		// include I/O definitions (port names, pin names, etc)

#include "uart.h"


// line edit buffer
#define RX_BUFSIZE 20

// adapter function to uart library
static int uart_putchar(char c, FILE *stream)
{
	if (c == '\n')
		uart_putchar('\r', stream);
	uart_putc(c);
	return 0;
}



static int uart_getchar(FILE *stream)
{
	
	uint8_t c;
	char *cp, *cp2;
	static char b[RX_BUFSIZE];
	static char *rxp;
	
	
	int val;
	

	if (rxp == 0)
	for (cp = b;;)
	{
		
		// get a character from uart lib
		do
		{
			val = uart_getc();
			//if (val >> 8 == UART_NO_DATA) return _FDEV_EOF;
			//if (val >> 8 != 0) return _FDEV_ERR;

		}while( val >> 8 != 0 );
		c =  val & 0xff;




		/* behaviour similar to Unix stty ICRNL */
		if (c == '\r')
		c = '\n';
		if (c == '\n')
		{
			*cp = c;
			uart_putchar(c, stream);
			rxp = b;
			break;
		}
		else if (c == '\t')
		c = ' ';

		if ((c >= (uint8_t)' ' && c <= (uint8_t)'\x7e') ||
		c >= (uint8_t)'\xa0')
		{
			if (cp == b + RX_BUFSIZE - 1)
			uart_putchar('\a', stream);
			else
			{
				*cp++ = c;
				uart_putchar(c, stream);
			}
			continue;
		}

		switch (c)
		{
			case 'c' & 0x1f:
			return -1;

			case '\b':
			case '\x7f':
			if (cp > b)
			{
				uart_putchar('\b', stream);
				uart_putchar(' ', stream);
				uart_putchar('\b', stream);
				cp--;
			}
			break;

			case 'r' & 0x1f:
			uart_putchar('\r', stream);
			for (cp2 = b; cp2 < cp; cp2++)
			uart_putchar(*cp2, stream);
			break;

			case 'u' & 0x1f:
			while (cp > b)
			{
				uart_putchar('\b', stream);
				uart_putchar(' ', stream);
				uart_putchar('\b', stream);
				cp--;
			}
			break;

			case 'w' & 0x1f:
			while (cp > b && cp[-1] != ' ')
			{
				uart_putchar('\b', stream);
				uart_putchar(' ', stream);
				uart_putchar('\b', stream);
				cp--;
			}
			break;
		}
	}

	c = *rxp++;
	if (c == '\n')
	rxp = 0;

	return c;
}



static FILE mystdout_i = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);


void attachUART(void)
{
	uart_init( UART_BAUD_SELECT(57600,F_CPU));
	stdout = stdin = &mystdout_i;
}



void interactiveTestShell()
{

	char buf[20];
	uint16_t u16value;
	
	
	printf_P(PSTR("Test Shell Activated\n"));
	
	for (;;)
	{
		printf_P(PSTR("Enter command: "));
		
		if (fgets(buf, sizeof buf - 1, stdin) != NULL)
		{
			if (tolower(buf[0]) == 'q')
			break;

			switch (buf[0])
			{
				default:
				printf("Unknown command: %s\n", buf);

				printf_P(PSTR("w write port a(x)\n"));
				printf_P(PSTR("d dir port a(x)\n"));
				printf_P(PSTR("q quit\n"));
				printf_P(PSTR("r read port A\n"));
				
				
				printf("q quit\n");
				break;

				case 'w':
				if (sscanf(buf, "%*s %x", &u16value) > 0)
				{
                    PORTB=u16value & 0xff;

					printf("wrote data %02x\n", u16value);
				}
				else
				{
					printf("sscanf() failed\n");
				}

				break;

				case 'd':
				if (sscanf(buf, "%*s %x", &u16value) > 0)
				{
                    DDRB=u16value & 0xff;
					printf("wrote direction %02x\n", u16value);
				}
				else
				{
					printf("sscanf() failed\n");
				}

				break;

				case 'r':					
					printf("port status %02x\n",PINB);
				break;
				

			}//switch
		}
		else
		printf("nix\n");//if fgets
	}// for ;;
	
}


int main(int argc, char *argv[])
{
    
    attachUART();
    sei();
  
    printf("i am alive.. " __TIMESTAMP__ "\n") ;
    
    interactiveTestShell();

    DDRB |= (1<<7);
    while(1)
    {
        _delay_ms(100);
        PORTB |= (1<<7);
        _delay_ms(100);
        PORTB &= ~(1<<7);
    }

}
