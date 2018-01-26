
extern "C" {
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <math.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
}




// command handling code in c++
// this is just an example that c++ can be useful for embedded systems.
// it demonstrates also how to use stdio with the uart


// ------------------------------ base classes ---------------------------

// RegisteredCmd 
//   used for registration of commands
// and being able to call the CmdBase api from there


class RegisteredCmd {
        RegisteredCmd *chain;
    public:
        static RegisteredCmd *registry;
        RegisteredCmd() : chain(registry) {registry=this;}

        virtual void receive(char* p_buf)=0;
        virtual const char* id(void)=0;

        static void process_cmd(char* buf)
        {
            bool found=false;
            for( RegisteredCmd* p = RegisteredCmd::registry ; p!=0 ; p=p->chain)
            {
                    // find first space or end
                    char* pstr=buf;
                    int pos=0;
                    while (1)
                    {
                        if(*pstr == 0 || *pstr ==' ')
                            break;
                        pos++;
                        pstr++;
                    }
                    if (strncmp(buf, p->id() ,pos)==0 )
                    {
                        p->receive(buf );
                        found=true;
                        break;
                    }
            }
            if ( ! found)
            {
                printf("commands :\n");
                for( RegisteredCmd* p = RegisteredCmd::registry ; p!=0 ; p=p->chain)
                    printf(" %s\n",p->id());
            }
        }
        
};


// base and specific commands implemented
// with the curiously recurring template pattern
// the specific command must 
//   - implement the parse  method used in the template. 
//   - implement an id method

template<typename T>
class CmdBase: public RegisteredCmd
{
	void (* m_fu) (T*);
  
public:
	CmdBase( void (* _fu) (T*) ):m_fu(_fu){}
	
	void receive(char* p_buf)
	{
		if ( ((T*)this)->parse(p_buf) )
            m_fu( (T*)this );   
        else
            usage();
		
	}
    void usage()
    {
        printf (" usage: %s\n",id() ); 
    }
};

// ------------------------------------ application  command classes -------------------

class SpecificCmd:public CmdBase< SpecificCmd >
{
 
public:
	uint32_t m_field_1;
	uint32_t m_field_2;

	SpecificCmd( void (* _cb) (SpecificCmd*)):CmdBase< SpecificCmd >(_cb){}

	bool parse(char* p_buffer){ return sscanf( p_buffer, "%*s %lx %lx", &m_field_1, &m_field_2 ) == 2;  }

	const char* id(void){ return "cmd1 <X32> <X32>"; }
};

class OtherSpecificCmd:public CmdBase< OtherSpecificCmd >
{
public:
	uint16_t m_other_field_1;
	uint16_t m_other_field_2;

	OtherSpecificCmd( void (* _cb) (OtherSpecificCmd*)):CmdBase< OtherSpecificCmd >(_cb){}

	bool parse(char* p_buffer) { return sscanf( p_buffer, "%*s %x %x", &m_other_field_1, &m_other_field_2 ) == 2;  }
    
	const char* id(void){ return "othercmd <X8> <X8>";}
};

//---------------------------------------- instanciation of commands ------------------------------

// a callback is specified using a lambda function
SpecificCmd my_cmd0( [] (SpecificCmd* m) { printf("SpecificCmd %08lx %08lx\n", m->m_field_1, m->m_field_2); } );
OtherSpecificCmd my_cmd1( [] (OtherSpecificCmd* m) { printf("OtherSpecificCmd %02x %02x\n", m->m_other_field_1, m->m_other_field_2); } );


//initialize command registry
RegisteredCmd* RegisteredCmd::registry = 0;

// ---------------------------------- uart stuff --------------------------------------------------------
extern "C" {
#include "uart.h"
}

// line edit buffer
#define RX_BUFSIZE 80

// adapter function to uart library
static int uart_putchar(char c, FILE *stream)
{
	if (c == '\n')
		uart_putchar('\r', stream);
	uart_putc(c);
	return 0;
}



// adapter function to uart library
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



FILE _stdoutref;

void attachUART(void)
{
	uart_init( UART_BAUD_SELECT(57600,F_CPU));

    _stdoutref.put=uart_putchar;
    _stdoutref.get=uart_getchar;
    _stdoutref.flags=_FDEV_SETUP_RW;
    _stdoutref.udata=0;    
    stdout = stdin = &_stdoutref;
     
}

// ---------------------------------------- main loop ------------------------------------------------

int main(int argc, char *argv[])
{
    // set up stdio to use the uart
    attachUART();
    sei();
  
    printf("i am alive.. " __TIMESTAMP__ "\n") ;    

    char buf[100];
    
	while(1)
    {
        if ( (fgets (buf , 100 , stdin) != NULL) &&
             (strlen(buf) !=0)	 )
        {   
            RegisteredCmd::process_cmd(buf);            
        }
    }
}
