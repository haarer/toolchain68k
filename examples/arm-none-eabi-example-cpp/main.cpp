
/* a simple blink example and serial out */
#include <stdint.h>
extern "C"
{
#include "stm32f10x.h"
#include "system.h"
#include "uart.h"
}
#include <stdio.h>
//#include <iostream>
#include <vector>



static volatile uint32_t sysTickCount = 0;

extern "C"
{
void SysTick_Handler(void)
{
	if (sysTickCount != 0) {
		sysTickCount--;
	}
}
}



void myDelay(uint32_t nTime)
{
	sysTickCount = nTime;
	while(sysTickCount != 0);
};


inline void pinMode(GPIO_TypeDef *port, uint8_t newPin, uint8_t mode)
{
    if (newPin < 8)
    {
        port->CRL &= ~(0b11 << (newPin << 2));
        port->CRL |= mode << (newPin << 2);
    }
    else
    {
        port->CRH &= ~(0b11 << ((newPin - 8) << 2));
        port->CRH |= mode << ((newPin - 8) << 2);
    }
}

inline void pinConfg(GPIO_TypeDef *port, uint8_t newPin, uint8_t config)
{
    if (newPin < 8)
    {
        port->CRL &= ~(0b11 << ((newPin << 2) + 2));
        port->CRL |= config << ((newPin << 2) + 2);
    }
    else
    {
        port->CRH &= ~(0b11 << (((newPin - 8) << 2) + 2));
        port->CRH |= config << (((newPin - 8) << 2) + 2);
    }
}

inline void togglePin(GPIO_TypeDef *port, uint8_t newPin) { port->ODR ^= 1 << newPin; }

inline void highPin(GPIO_TypeDef *port, uint8_t newPin) { port->BSRR = 1 << newPin; };

inline void lowPin(GPIO_TypeDef *port, uint8_t newPin) { port->BRR = 1 << newPin; };



enum MODE : uint8_t
    {
        INPUT = (uint8_t)0b00,
        OUTPUT_10MHZ,
        OUTPUT_2MHZ,
        OUTPUT_50MHZ
    };

    enum CNF : uint8_t
    {
        INPUT_ANALOG = (uint8_t)0b00,
        INPUT_FLOATING,
        INPUT_PULLUP_DOWN,
        INPUT_RESERVED,
        OUTPUT_PUSH_PULL = (uint8_t)0b00,
        OUTPUT_OPEN_DRAIN,
        OUTPUT_ALT_PUSH_PULL,
        OUTPUT_ALT_OPEN_DRAIN
    };


int main(void)
{

	static std::vector<int> vec;
    uartInit(115200);

	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; // enable port c

	pinMode(GPIOC,13,OUTPUT_2MHZ);
	pinConfg(GPIOC,13,OUTPUT_PUSH_PULL);


	SysTick_Config(SystemCoreClock/1000);

	while (1)
	{

		highPin(GPIOC,13);
		myDelay(100);

		lowPin(GPIOC,13);
		myDelay(100);


        //does not fit into flash
		//std::cout << "hello" << std::endl;
		vec.push_back(0);
		printf("hello vector contains %d elements \n",vec.size());
		if (vec.size() > 100)
		{
			vec.clear();
            printf("enough is enough, resetting\n ");
		}

	}
}

