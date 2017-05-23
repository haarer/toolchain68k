

#include <avr/io.h>
#include <util/delay.h>
int main(int argc, char *argv[])
{
    DDRB |= (1<<7);
    while(1)
    {
        _delay_ms(1000);
        PORTB |= (1<<7);
        _delay_ms(1000);
        PORTB &= ~(1<<7);
    }
}
