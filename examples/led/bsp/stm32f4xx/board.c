#include <stddef.h>

#include "custom-assert.h"
#include "board.h"
#include "gpio.h"
#include "gpio-name.h"
#include "cmsis-device.h"

/* LEDs gpios */
static GpioHandle_t m_gpioLedGreen;
static GpioHandle_t m_gpioLedYellow;
static GpioHandle_t m_gpioLedWhite;

/* LEDs */
static Led_t m_ledGreen;
static Led_t m_ledYellow;
static Led_t m_ledWhite;

static LedBlink_t m_ledBlink[BOARD_LED_COUNT];

void Board_Init(void)
{
    NVIC_SetPriorityGrouping(0);

    const GpioOps_t* ops = GpioGetOps();

    m_gpioLedGreen.ops = ops;
    m_gpioLedWhite.ops = ops;
    m_gpioLedYellow.ops = ops;

    LedInit(&m_ledGreen, &m_gpioLedGreen, PA_5);
    LedInit(&m_ledYellow, &m_gpioLedYellow, PC_3);
    LedInit(&m_ledWhite, &m_gpioLedWhite, PC_4);

    m_ledBlink[BOARD_LED_GREEN].led = &m_ledGreen;
    m_ledBlink[BOARD_LED_GREEN].periodMs = 100;

    m_ledBlink[BOARD_LED_WHITE].led = &m_ledWhite;
    m_ledBlink[BOARD_LED_WHITE].periodMs = 250;

    m_ledBlink[BOARD_LED_YELLOW].led = &m_ledYellow;
    m_ledBlink[BOARD_LED_YELLOW].periodMs = 1000;
}

LedBlink_t* Board_GetLed(BOARD_LED_ID id)
{
    ASSERT(id < BOARD_LED_COUNT);

    return &m_ledBlink[id];
}

