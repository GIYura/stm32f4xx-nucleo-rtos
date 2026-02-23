#include "FreeRTOS.h"
#include "task.h"
#include "board.h"
#include "queue.h"

static xQueueHandle m_queue = NULL;

static volatile uint32_t m_idleCycleCount = 0;

static void TxTask(void* pvParams)
{
    (void)pvParams;

    uint32_t value = 0;

    for (;;)
    {
        BaseType_t status = xQueueSend(m_queue, &value, pdMS_TO_TICKS(100));
        if (status == pdTRUE)
        {
            value++;
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

static void RxTask(void* pvParams)
{
    (void)pvParams;

    uint32_t expected = 0;

    for (;;)
    {
        uint32_t value = 0;

        BaseType_t status = xQueueReceive(m_queue, &value, portMAX_DELAY);
        configASSERT(status == pdTRUE);

        configASSERT(value == expected);
        expected++;
    }
}

int main(void)
{
    Board_Init();

    SEGGER_SYSVIEW_Conf();
    SEGGER_SYSVIEW_Start();

    m_queue = xQueueCreate(1, sizeof(uint8_t));
    if (m_queue != NULL)
    {
        BaseType_t t1 = xTaskCreate(TxTask, "TX TASK", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
        BaseType_t t2 = xTaskCreate(RxTask, "RX TASK", configMINIMAL_STACK_SIZE, NULL, 2, NULL);

        if (t1 == pdTRUE && t2 == pdTRUE)
        {
            vTaskStartScheduler();
        }
    }

    for(;;);
    /* never reach here */

    return 0;
}

void vApplicationIdleHook(void)
{
    m_idleCycleCount++;
}

