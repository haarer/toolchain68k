
/** UART_H */
#ifndef UART_H
#define UART_H


#include "stm32f10x.h"
#include "system.h"
extern char uartGetch(void);
extern void uartPutch(char ch);
extern void USART1_IRQHandler(void);



/**
 * @brief Initialize USART1 peripheral
 * 
 * @param baudrate baudrate to be configurate
 */
static inline void uartInit(uint32_t baudrate)
{
    // enable clock for GPIOA and USART1
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN | RCC_APB2ENR_IOPAEN;

    // reset pin configurations for PA9 and PA10
    GPIOA->CRH &= ~(GPIO_CRH_MODE10 | GPIO_CRH_MODE9 | GPIO_CRH_CNF10 | GPIO_CRH_CNF9);

    // PA9 as Output@50Hz Alternate function
    GPIOA->CRH |= GPIO_CRH_MODE9_0 | GPIO_CRH_MODE9_1 | GPIO_CRH_CNF9_1;

    // PA10 as floating input
    GPIOA->CRH |= GPIO_CRH_CNF10_0;

    uint32_t baud = (uint32_t)(SystemCoreClock / baudrate);

    USART1->BRR = baud;

    // transmitter enable, receiver enable, receiver interrupt enable and USART enable
    USART1->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE | USART_CR1_UE;

    // enable USART1 interrupt
    NVIC_EnableIRQ(USART1_IRQn);
}


#endif
