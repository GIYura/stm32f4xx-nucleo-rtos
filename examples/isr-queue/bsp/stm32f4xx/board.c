#include <stddef.h>

#include "button.h"
#include "custom-assert.h"
#include "board.h"
#include "gpio.h"
#include "gpio-name.h"
#include "cmsis-device.h"
#include "timer.h"
#include "sw-timer.h"

#define HW_TIMER_TIMEOUT_MS             1
#define BUTTON_DEBOUNCE_TIMEOUT_MS      20

#define DWT_CTRL    (*(volatile uint32_t*)0xE0001000)

/* Button gpio */
static GpioHandle_t m_gpioButtonUser;

/* Button */
static Button_t m_buttonUser;

/* LEDs gpios */
static GpioHandle_t m_gpioLedGreen;
static GpioHandle_t m_gpioLedYellow;
static GpioHandle_t m_gpioLedWhite;

/* LEDs */
static Led_t m_ledGreen;
static Led_t m_ledYellow;
static Led_t m_ledWhite;

static LedBlink_t m_ledBlink[BOARD_LED_COUNT];

/* Debounce timer (based on HW timer) */
static SwTimer_t debounceTimer;

/* HW timer */
static TimerHandle_t m_hwTimer;

void Board_Init(void)
{
    NVIC_SetPriorityGrouping(0);

    const GpioOps_t* gpioOps = GpioGetOps();
    const TimerOps_t* hwTimerOps = TimerGetOps();

    m_gpioLedGreen.ops = gpioOps;
    m_gpioLedWhite.ops = gpioOps;
    m_gpioLedYellow.ops = gpioOps;

    m_gpioButtonUser.ops = gpioOps;
    m_gpioButtonUser.ops->open(&m_gpioButtonUser, PC_13, PIN_MODE_INPUT, PIN_TYPE_PULL_UP, PIN_STRENGTH_HIGH, PIN_CONFIG_PUSH_PULL, PIN_STATE_HIGH);

    LedInit(&m_ledGreen, &m_gpioLedGreen, PA_5);
    LedInit(&m_ledYellow, &m_gpioLedYellow, PC_3);
    LedInit(&m_ledWhite, &m_gpioLedWhite, PC_4);

    ButtonInit(&m_buttonUser, &m_gpioButtonUser, &debounceTimer, BUTTON_DEBOUNCE_TIMEOUT_MS);

    m_ledBlink[BOARD_LED_GREEN].led = &m_ledGreen;
    m_ledBlink[BOARD_LED_GREEN].periodMs = 100;

    m_ledBlink[BOARD_LED_WHITE].led = &m_ledWhite;
    m_ledBlink[BOARD_LED_WHITE].periodMs = 150;

    m_ledBlink[BOARD_LED_YELLOW].led = &m_ledYellow;
    m_ledBlink[BOARD_LED_YELLOW].periodMs = 250;

    m_hwTimer.ops = hwTimerOps;

    m_hwTimer.ops->open(&m_hwTimer, HW_TIMER_TIMEOUT_MS);

    m_hwTimer.ops->interrupt(&m_hwTimer, SwTimerTick, 6);

    m_hwTimer.ops->start(&m_hwTimer);

    DWT_CTRL |= (1 << 0);
}

LedBlink_t* Board_GetLed(BOARD_LED_ID id)
{
    ASSERT(id < BOARD_LED_COUNT);

    return &m_ledBlink[id];
}

Button_t* Board_GetButton(BOARD_BUTTON_ID id)
{
    ASSERT(id < BOARD_BUTTON_COUNT);

    switch (id)
    {
    case BOARD_BUTTON_USER:
        return &m_buttonUser;
        break;

    default:
        /* never reach here */
        return NULL;
        break;
    }
}

