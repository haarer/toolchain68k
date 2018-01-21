
// example is for arduino mega

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>


// change this define to select impementation
#define TEMPLATE_SWITCH
//#define CLASSIC

#ifdef TEMPLATE_SWITCH
#define TEMPLATEBASED
typedef enum {in,out} PinDir_T;

template<uint8_t PinNo>
class IOPin
{
	public:
	IOPin(){};
	void SetDir(PinDir_T dir)
	{
		switch(PinNo)
		{
			case  2: DDRE |= (1<<4); break; // pin  2 is PE 4 on arduino mega
			case 12: DDRB |= (1<<6); break; // pin 12 is PB 6 on arduino mega
			case 13: DDRB |= (1<<7); break; // pin 13 is PB 7 on arduino mega
		}
	}
	void SetHigh(void)
	{
		switch(PinNo)
		{
			case  2: PORTE |= (1<<4); break;
			case 12: PORTB |= (1<<6); break;
			case 13: PORTB |= (1<<7); break;
		}
	}
	void SetLow(void)
	{
		switch(PinNo)
		{
			case  2: PORTE &= ~(1<<4); break;
			case 12: PORTB &= ~(1<<6); break;
			case 13: PORTB &= ~(1<<7); break;
		}
	}
	bool State(void)
	{
		switch(PinNo)
		{
			case  2: return (PORTE & (1<<4) ) == (1<<4); break;
			case 12: return (PORTB & (1<<6) ) == (1<<6); break;
			case 13: return (PORTB & (1<<7) ) == (1<<7); break;
		}
	}
};
#endif


int main(void)
{
#ifdef CLASSIC
	DDRB |= (1<<7);

	while(  (  ( PORTB & (1<<6) ) !=(1<<6) ) )
		;

	while(1)
	{
        	_delay_ms(100);
		PORTB |= (1<<7);
		_delay_ms(100);
		PORTB &= ~(1<<7);
	}
#endif

#ifdef TEMPLATEBASED
	IOPin<13> ledpin;
	IOPin<12> testpin;
	ledpin.SetDir(out);

	while(0 == testpin.State())
		;

	while(1)
	{
        	_delay_ms(100);
		ledpin.SetHigh();
		_delay_ms(100);
		ledpin.SetLow();
	}
#endif
}
