#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "board.h"
#include "button.h"
#include "led.h"

static volatile uint32_t m_idleCycleCount = 0;
static uint32_t m_buttonUserCounter = 0;

static TaskHandle_t m_LedGreenTaskHandle = NULL;
static TaskHandle_t m_LedYellowTaskHandle = NULL;

static xQueueHandle m_queue = NULL;

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
        BaseType_t status = xQueueReceive(m_queue, &pressCount, portMAX_DELAY);
        configASSERT(status == pdTRUE);

        if (pressCount != 0)
        {
            vTaskSuspend(m_LedGreenTaskHandle);
            vTaskSuspend(m_LedYellowTaskHandle);
        }
    }
}

static void OnButtonUser(void)
{
    ++m_buttonUserCounter;

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    BaseType_t status = xQueueSendFromISR(m_queue, &m_buttonUserCounter, &xHigherPriorityTaskWoken);
    configASSERT(status == pdTRUE);
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

    m_queue = xQueueCreate(1, sizeof(uint32_t));

    Button_t* buttonUser = Board_GetButton(BOARD_BUTTON_USER);
    ButtonRegisterHandler(buttonUser, OnButtonUser);

    LedBlink_t* ledWhite = Board_GetLed(BOARD_LED_WHITE);
    LedBlink_t* ledGreen = Board_GetLed(BOARD_LED_GREEN);
    LedBlink_t* ledYellow = Board_GetLed(BOARD_LED_YELLOW);

    LedOff(ledWhite->led);
    LedOn(ledGreen->led);
    LedOn(ledYellow->led);

    BaseType_t t1 = xTaskCreate(ButtonTask, "Button", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    BaseType_t t2 = xTaskCreate(LedBlinkTask, "Green", configMINIMAL_STACK_SIZE, ledGreen, 1, &m_LedGreenTaskHandle);
    BaseType_t t3 = xTaskCreate(LedBlinkTask, "Yellow", configMINIMAL_STACK_SIZE, ledYellow, 1, &m_LedYellowTaskHandle);

    if (t1 == pdTRUE && t2 == pdTRUE && t3 == pdTRUE && m_queue != NULL)
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

