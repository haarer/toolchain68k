
#ifndef __SYSTEM_STM32F10X_H
#define __SYSTEM_STM32F10X_H

#ifdef __cplusplus
 extern "C" {
#endif 

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

extern uint32_t SystemCoreClock;          /*!< System Clock Frequency (Core Clock) */

extern void SystemInit(void);


#ifdef __cplusplus
}
#endif

#endif /*__SYSTEM_STM32F10X_H */
