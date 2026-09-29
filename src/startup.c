/* Reset handler and vector table for the STM32F429. */
#include <stdint.h>
#include "stm32f429.h"

extern uint32_t _sidata, _sdata, _edata, _sbss, _ebss, _estack;
extern int main(void);

void Reset_Handler(void);
void Default_Handler(void);

/* FreeRTOS port handlers (renamed to these via FreeRTOSConfig.h). */
void SVC_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

void NMI_Handler(void)        __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void)  __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void)  __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void)   __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void)   __attribute__((weak, alias("Default_Handler")));

/* 16 core exceptions + 91 peripheral IRQs; the demo uses no peripheral IRQs,
 * so they all fall through to Default_Handler. */
#define NUM_IRQS 91

__attribute__((section(".isr_vector"), used))
void (* const vector_table[16 + NUM_IRQS])(void) = {
    (void (*)(void))&_estack,
    Reset_Handler,
    NMI_Handler,
    HardFault_Handler,
    MemManage_Handler,
    BusFault_Handler,
    UsageFault_Handler,
    0, 0, 0, 0,
    SVC_Handler,
    DebugMon_Handler,
    0,
    PendSV_Handler,
    SysTick_Handler,
    [16 ... 16 + NUM_IRQS - 1] = Default_Handler,
};

void Reset_Handler(void)
{
    uint32_t *src = &_sidata;
    for (uint32_t *dst = &_sdata; dst < &_edata; )
        *dst++ = *src++;
    for (uint32_t *dst = &_sbss; dst < &_ebss; )
        *dst++ = 0;

    SCB_VTOR = (uint32_t)vector_table;
    SCB_CPACR |= (0xFu << 20);  /* Full access to CP10/CP11 (FPU) */
    __asm volatile ("dsb\n isb");

    main();
    for (;;) { }
}

void Default_Handler(void)
{
    __asm volatile ("bkpt #0");
    for (;;) { }
}
