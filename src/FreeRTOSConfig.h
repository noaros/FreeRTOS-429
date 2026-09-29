#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#define configCPU_CLOCK_HZ                      168000000UL
#define configTICK_RATE_HZ                      1000
#define configUSE_PREEMPTION                    1
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 1
#define configUSE_TICKLESS_IDLE                 0
#define configMAX_PRIORITIES                    5
#define configMINIMAL_STACK_SIZE                128
#define configMAX_TASK_NAME_LEN                 12
#define configTICK_TYPE_WIDTH_IN_BITS           TICK_TYPE_WIDTH_32_BITS
#define configIDLE_SHOULD_YIELD                 1
#define configUSE_MUTEXES                       1
#define configQUEUE_REGISTRY_SIZE               0
#define configUSE_TIME_SLICING                  1

/* Memory (heap_4) */
#define configSUPPORT_DYNAMIC_ALLOCATION        1
#define configSUPPORT_STATIC_ALLOCATION         0
#define configTOTAL_HEAP_SIZE                   (32 * 1024)

/* Hooks and debugging */
#define configUSE_IDLE_HOOK                     0
#define configUSE_TICK_HOOK                     0
#define configUSE_MALLOC_FAILED_HOOK            1
#define configCHECK_FOR_STACK_OVERFLOW          2

/* Software timers */
#define configUSE_TIMERS                        1
#define configTIMER_TASK_PRIORITY               (configMAX_PRIORITIES - 1)
#define configTIMER_QUEUE_LENGTH                5
#define configTIMER_TASK_STACK_DEPTH             configMINIMAL_STACK_SIZE

/* Cortex-M4 interrupt priorities: the STM32F4 implements 4 priority bits. */
#define configPRIO_BITS                         4
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY      15
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 5
#define configKERNEL_INTERRUPT_PRIORITY \
    (configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS))
#define configMAX_SYSCALL_INTERRUPT_PRIORITY \
    (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS))

/* Optional API */
#define INCLUDE_vTaskDelay                      1
#define INCLUDE_vTaskDelayUntil                 1
#define INCLUDE_xTaskGetTickCount               1
#define INCLUDE_uxTaskGetStackHighWaterMark     1

#define configASSERT(x) do { if (!(x)) { __asm volatile ("cpsid i"); for (;;) { } } } while (0)

/* Map the FreeRTOS port handlers onto the vector table names. */
#define vPortSVCHandler     SVC_Handler
#define xPortPendSVHandler  PendSV_Handler
#define xPortSysTickHandler SysTick_Handler

#endif
