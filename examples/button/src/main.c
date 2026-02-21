#include "FreeRTOS.h"
#include "task.h"
#include "board.h"
#include "button.h"
#include "led.h"

static volatile uint32_t m_idleCycleCount = 0;
static uint32_t m_buttonUserCounter = 0;

static TaskHandle_t m_buttonTaskHandle = NULL;

static void LedBlinkTask(void *pvParams)
{
    LedBlink_t* l = (LedBlink_t*)pvParams;

    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;)
    {
        LedToggle(l->led);

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(l->periodMs));
    }
}

static void ButtonTask(void *pvParams)
{
    uint32_t pressCount = 0;

    for (;;)
    {
        uint32_t count = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        pressCount += count;

        if (pressCount % 5 == 0)
        {
            pressCount = 0;
            LedToggle(Board_GetLed(BOARD_LED_WHITE)->led);
        }
    }
}

static void OnButtonUser(void)
{
    ++m_buttonUserCounter;

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(m_buttonTaskHandle, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

int main(void)
{
    Board_Init();

#if 0
    SEGGER_UART_init(9600);
#endif

    SEGGER_SYSVIEW_Conf();
    SEGGER_SYSVIEW_Start();

    Button_t* buttonUser = Board_GetButton(BOARD_BUTTON_USER);
    ButtonRegisterHandler(buttonUser, OnButtonUser);

    LedBlink_t* ledWhite = Board_GetLed(BOARD_LED_WHITE);
    LedBlink_t* ledGreen = Board_GetLed(BOARD_LED_GREEN);
    LedBlink_t* ledYellow = Board_GetLed(BOARD_LED_YELLOW);

    LedOff(ledWhite->led);
    LedOn(ledGreen->led);
    LedOn(ledYellow->led);

    BaseType_t t1 = xTaskCreate(ButtonTask, "Button", configMINIMAL_STACK_SIZE, NULL, 2, &m_buttonTaskHandle);
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

