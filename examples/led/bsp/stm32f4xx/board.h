#ifndef BOARD_H
#define BOARD_H

#include "led.h"

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

#endif /* BOARD_H */
