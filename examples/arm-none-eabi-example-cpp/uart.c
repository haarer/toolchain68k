
#include "stm32f10x.h"

#include <uart.h>

char uartGetch(void)
{
  return 0;
}

void uartPutch(char c)
{
    // wait for tx data register to be empty
    while (!(USART1->SR & USART_SR_TXE))
        ;
    USART1->DR = 0x000000ff & c;
}

void uartPuts(const char *ch)
{
    while (*ch)
    {
        uartPutch(*ch);
        ch++;
    }
}


void USART1_IRQHandler(void)
{
    if (USART1->SR & USART_SR_RXNE)
    {
        // this clears RXNE flag
        uartPutch(USART1->DR & 0xff);
    }
}
