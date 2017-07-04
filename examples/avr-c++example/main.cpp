
extern "C" {
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>

#include <math.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
}




// example of message handling code in c++
// there is some code missing for crc and resynchronization in the middle of a data stream
// normally this is achieved by a timing (pause between messages) or a transport encoding
// this is just an example that c++ can be useful for embedded systems.
// it demonstrates also how to use stdio with the uart

#define UART_BAUD_SELECT(baudRate,xtalCpu)  (((xtalCpu) + 8UL * (baudRate)) / (16UL * (baudRate)) -1UL)




// --------------------- primitive USART (unbuffered sending only) ------------------------------
class USART
{
    
    
    public:
    USART(uint32_t rate)
    {
        uint32_t baud_reg_value = UART_BAUD_SELECT(rate, F_CPU);
        // Set baud rate 
        if (baud_reg_value & 0x8000) 
        {
            UCSR0A = (1 << U2X0);
            baud_reg_value &= ~0x8000;
        }

        UBRR0H = (uint8_t) (baud_reg_value >> 8);
        UBRR0L = (uint8_t) baud_reg_value;

        // Enable USART  transmitter 
        UCSR0B =  (1 << TXEN0);
        
    }
    // same as above but with stdio 
    
    
    
    void send(uint8_t data) 
    {
        // Wait for empty transmit buffer 
        while (!( UCSR0A & (1 << UDRE0))){}

        // Put data into buffer, sends the data
         UDR0 = data;
	}
    
    void send(const uint8_t* p_data, size_t length) 
    {
		while (length != 0) 
        {
			send(*p_data);
			length--;
			p_data++;
		}
	}

	//returns 0 if no byte available, 1 if byte available and stores byte at pointer
	uint8_t receivebyte(uint8_t* p_data)
    {
        //tbd :)
        
        return 0;   
    }
    
};

// ------------------------------ message base classes ---------------------------

// the MsgIf is used for storing pointers to the messages
//and being able to call the MsgBase api from there


typedef enum{MSG_OK=0,MSG_ERR_FRAME_SHORT=1,MSG_ERR_CRC=2} msg_res_t;

class MsgIf
{
public:
	virtual void send(uint8_t* p_buf)=0;
	virtual msg_res_t receive(uint8_t* p_buf,uint8_t* p_buf_last)=0;
	virtual const uint8_t len(void)=0;
	virtual const uint8_t id(void)=0;

};

//compute a crc for a buffer of length -1 the last byte is the crc to be checked against
//0 : fail 
//1 : ok
uint8_t check_crc(uint8_t* p_buf,uint8_t size)
{
    return 1;
}

// base and specifig messages implemented using static polymorphism 
// with the curiously recurring template pattern
// the specific messages must implement the to_buffer and from_buffer methods
// just because they are used in the template. The compiler knows this because of the
// cast of the this pointer to the template parameter type

template<typename T>
class MsgBase: public MsgIf
{
	void (* m_fu) (T*);
public:
	MsgBase( void (* _fu) (T*) ):m_fu(_fu) {}
	
	void send(uint8_t* p_buf)
	{
		p_buf = ((T*)this)->to_buffer(p_buf);
	}

	msg_res_t receive(uint8_t* p_buf,uint8_t* p_buf_last)
	{
        // second byte : length
        uint8_t len = *(p_buf+1);

        if( p_buf + len > p_buf_last) 
            return MSG_ERR_FRAME_SHORT;

        if( ! check_crc(p_buf,len) )
            return MSG_ERR_CRC;
        
        // 3rd byte user data
		p_buf = ((T*)this)->from_buffer(p_buf+2);

		m_fu( (T*)this );
        return MSG_OK;
	}


};

// ------------------------------------ application  message classes -------------------

class SpecificMsg:public MsgBase<SpecificMsg>
{
public:
	uint32_t m_field_1;

	SpecificMsg( void (* _cb) (SpecificMsg*)):MsgBase<SpecificMsg>(_cb){}


	// we should pass the pointer by reference and save increment at the caller
	// and drop the return value
	uint8_t* to_buffer(uint8_t* p_buffer)
	{
		//fixme: not the best solution, just an example 
		// that here the message is marshalled
		// most significant byte first (motorola order)
		*p_buffer = ( m_field_1 >> 24 ) & 0xff; p_buffer ++;
		*p_buffer = ( m_field_1 >> 16 ) & 0xff; p_buffer ++;
		*p_buffer = ( m_field_1 >> 8  ) & 0xff; p_buffer ++;
		*p_buffer = ( m_field_1 >> 0  ) & 0xff; p_buffer ++;
		return p_buffer;

	}

	uint8_t* from_buffer(uint8_t* p_buffer)
	{
		// assuming motorola order
        
		m_field_1  = *p_buffer; p_buffer ++; m_field_1 = m_field_1 << 8;
		m_field_1 += *p_buffer; p_buffer ++; m_field_1 = m_field_1 << 8;
		m_field_1 += *p_buffer; p_buffer ++; m_field_1 = m_field_1 << 8;
		m_field_1 += *p_buffer; p_buffer ++;
		return p_buffer;
	}

	const uint8_t len(void)
	{
		return 1+1+4+1;
	}

	const uint8_t id(void)
	{
		return 1;
	}
	

};

class OtherSpecificMsg:public MsgBase<OtherSpecificMsg>
{
public:
	uint8_t m_other_field_1;

	OtherSpecificMsg( void (* _cb) (OtherSpecificMsg*)):MsgBase<OtherSpecificMsg>(_cb){}


	// we should pass the pointer by reference and save increment at the caller
	// and drop the return value
	uint8_t* to_buffer(uint8_t* p_buffer)
	{
		//fixme: not the best solution, just an example 
		// that here the message is marshalled
		// most significant byte first (motorola order)

		*p_buffer = m_other_field_1 & 0xff; p_buffer ++;
		return p_buffer;

	}

	uint8_t* from_buffer(uint8_t* p_buffer)
	{
		// assuming motorola order

		m_other_field_1 = *p_buffer; p_buffer ++;
		return p_buffer;
	}

	const uint8_t len(void)
	{
		return 1+1+1+1;
	}

	const uint8_t id(void)
	{
		return 2;
	}
	

};

//---------------------------------------- instanciation of messages ------------------------------

// a callback is specified using a lambda function
SpecificMsg my_msg0( [] (SpecificMsg* m) { printf("SpecificMsg %08lx\n",m->m_field_1); } );
OtherSpecificMsg my_msg1( [] (OtherSpecificMsg* m) { printf("OtherMsg %02x\n",m->m_other_field_1); } );

// a container for messages to iterate over
MsgIf* messages[] = {&my_msg0, &my_msg1};

USART my_usart(57600);


// ----------------------   functions for stdio on uart  --------------------------------------------
int _putchar(char c, FILE *stream)
{
    my_usart.send((uint8_t*)&c,1);
    return 0;
}
	
    // rx not  implemented 
int _getchar(FILE *stream)
{
    int c;
    my_usart.receivebyte((uint8_t*)&c);
    return 0;
}

// ---------------------------------------- main loop ------------------------------------------------

int main(int argc, char *argv[])
{
    // set up stdio to use the uart
    FILE _stdoutref;

    _stdoutref.put=_putchar;
    _stdoutref.get=_getchar;
    _stdoutref.flags=_FDEV_SETUP_RW;
    _stdoutref.udata=0;    
    stdout = stdin = &_stdoutref;
        
	printf("Hello world\n");
    
    // test buffer with message data
    uint8_t buf[]={ 0x02,0x04,0x12,0xff, 
                    0x01,0x07,0x34,0x56,0x78,0x9A,0xff};
    

    uint8_t* p_buf=buf;
    uint8_t* p_endbuf=buf+ sizeof(buf);

	while(1)
	{     
		for(unsigned int i = 0 ; i < sizeof(messages)/sizeof(MsgIf*) ; i++)
		{
            printf("decode msg %d id =%d\n",i,messages[i]->id());
			// stupid stuff - just call the interface
			//messages[i]->send(p_buf);
			//p_buf += messages[i]->size();
     
            if (*p_buf == messages[i]->id() )
            {
                printf(" id ok\n");
                msg_res_t res = messages[i]->receive(p_buf, p_endbuf );
                if (res == MSG_OK)
                {
                    // advance buffer and restart processing
                    p_buf += messages[i]->len();
                    break; // break the for loop
                }
                else
                {
                    if (res == MSG_ERR_CRC)
                    {
                        // fixme: this is incomplete
                        printf("err crc\n"); // advance buffer and restart processing
                        p_buf += messages[i]->len();
                        break; // break the for loop
                    }

                    
                    if (res == MSG_ERR_FRAME_SHORT)
                    {
                        // fixme: this is incomplete
                        printf("err frame short\n"); // wait for more data
                        break; // break the for loop
                    }
                }
            }
        }
        
        if (p_buf >= p_endbuf)
            p_buf=buf;
	}
  //return 0;
}
