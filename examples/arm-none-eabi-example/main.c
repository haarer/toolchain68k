
/* a simple blink example */
#include "stm32f10x.h"

GPIO_InitTypeDef GPIO_InitStructure;

static volatile uint32_t sysTickCount = 0;


void SysTick_Handler()
{
	if (sysTickCount != 0) {
		sysTickCount--;
	}
}


void myDelay(uint32_t nTime)
{
	sysTickCount = nTime;
	while(sysTickCount != 0);
};

int main(void)
{

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	SysTick_Config(SystemCoreClock/1000);
	
	while (1)
	{

		GPIOC->BSRR = GPIO_Pin_13;
		myDelay(200);

		GPIOC->BRR  = GPIO_Pin_13;
		myDelay(200);
	}
}

#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif

