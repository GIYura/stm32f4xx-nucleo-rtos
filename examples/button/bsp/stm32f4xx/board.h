#ifndef BOARD_H
#define BOARD_H

#include "led.h"
#include "button.h"

typedef struct
{
    Led_t* led;
    uint32_t periodMs;
} LedBlink_t;

typedef enum
{
    BOARD_LED_GREEN,
    BOARD_LED_WHITE,
    BOARD_LED_YELLOW,
    BOARD_LED_COUNT
} BOARD_LED_ID;

typedef enum
{
    BOARD_BUTTON_USER,
    BOARD_BUTTON_COUNT
} BOARD_BUTTON_ID;

/*Brief: Board initialization
* [in] - none
* [out] - none
* */
void Board_Init(void);

/*Brief: Get board LED
* [in] - id  - LED id
* [out] - pointer to LED object
* */
LedBlink_t* Board_GetLed(BOARD_LED_ID id);

/*Brief: Get board button
* [in] - id  - button id
* [out] - pointer to button object
* */
Button_t* Board_GetButton(BOARD_BUTTON_ID id);

#endif /* BOARD_H */
