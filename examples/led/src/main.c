#include "FreeRTOS.h"
#include "task.h"
#include "board.h"
#include "led.h"

static volatile uint32_t m_idleCycleCount = 0;

static void LedBlinkTask(void *pvParams)
{
    LedBlink_t* l = (LedBlink_t*)pvParams;

    for (;;)
    {
        LedToggle(l->led);

        //taskYIELD();
        //vTaskDelay(pdMS_TO_TICKS(l->periodMs));
    }
}

int main(void)
{
    Board_Init();

    SEGGER_SYSVIEW_Conf();
    SEGGER_SYSVIEW_Start();

    LedBlink_t* ledWhite = Board_GetLed(BOARD_LED_WHITE);
    LedBlink_t* ledGreen = Board_GetLed(BOARD_LED_GREEN);
    LedBlink_t* ledYellow = Board_GetLed(BOARD_LED_YELLOW);

    BaseType_t t1 = xTaskCreate(LedBlinkTask, "White", configMINIMAL_STACK_SIZE, ledWhite, 1, NULL);
    BaseType_t t2 = xTaskCreate(LedBlinkTask, "Green", configMINIMAL_STACK_SIZE, ledGreen, 1, NULL);
    BaseType_t t3 = xTaskCreate(LedBlinkTask, "Yellow", configMINIMAL_STACK_SIZE, ledYellow, 1, NULL);

    if (t1 == pdTRUE && t2 == pdTRUE && t3 == pdTRUE)
    {
        vTaskStartScheduler();
    }

    for(;;);
    /* never reach here */

    return 0;
}

void vApplicationIdleHook(void)
{
    m_idleCycleCount++;
}

