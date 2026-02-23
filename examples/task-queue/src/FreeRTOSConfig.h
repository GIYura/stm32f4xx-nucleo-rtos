#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include "stm32f4xx.h"

/* CPU & Tick */
#define configCPU_CLOCK_HZ                              (SystemCoreClock)
#define configTICK_RATE_HZ                              (1000U)
#define configTICK_TYPE_WIDTH_IN_BITS                   TICK_TYPE_WIDTH_32_BITS

/* Tasks */
#define configUSE_PREEMPTION                            1
#define configMAX_PRIORITIES                            5
#define configMINIMAL_STACK_SIZE                        128
#define configMAX_TASK_NAME_LEN                         10
#define configIDLE_SHOULD_YIELD                         1
#define configUSE_TIME_SLICING                          0
#define INCLUDE_vTaskSuspend                            1

/* Memory */
#define configSUPPORT_DYNAMIC_ALLOCATION                1
#define configSUPPORT_STATIC_ALLOCATION                 0
#define configTOTAL_HEAP_SIZE                           (4096)

/* NVIC priorities */
#define configPRIO_BITS                                 __NVIC_PRIO_BITS
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY         15
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY    5

#define configKERNEL_INTERRUPT_PRIORITY                 (configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS))
#define configMAX_SYSCALL_INTERRUPT_PRIORITY            (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS))

#define INCLUDE_vTaskDelay                              1
#define INCLUDE_vTaskDelayUntil                         1
#define INCLUDE_vTaskDelete                             1
#define INCLUDE_vTaskPrioritySet                        1
#define INCLUDE_xTaskGetCurrentTaskHandle               1

#define INCLUDE_xTaskGetIdleTaskHandle                  1
#define INCLUDE_pxTaskGetStackStart                     1

/* Hooks */
#define configUSE_IDLE_HOOK                             1
#define configUSE_TICK_HOOK                             0
#define configCHECK_FOR_STACK_OVERFLOW                  0

/* define strong functions */
/* launch first task triggered by SVC instruction */
#define vPortSVCHandler                                 SVC_Handler

/* used for context switching between tasks */
#define xPortPendSVHandler                              PendSV_Handler

/* tick management triggered periodically by SysTick timer of ARM */
#define xPortSysTickHandler                             SysTick_Handler

#include "SEGGER_SYSVIEW_FreeRTOS.h"

#endif /* FREERTOS_CONFIG_H */
