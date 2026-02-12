### Description

LED blinky under FreeRTOS control

### Structure

- `bsp/config` - platform-specific make file
- `bsp/stm32f4xx` - board-specific files
- `scripts` - contains bash script to create build
- `src/main.c` - applcation code
- `src/mFreeRTOSConfig.h` - freeRTOS configuration file
- `uSDK` - link to shared SDK
- `makefile` - project main make file

### Set up

Target: NUCLEO-F411RE

### Connection 

| Nucleo pin   | GPIO   | LED    |
|--------------|--------|--------|
| CN7 (37)     | PC3    | Yellow |
| CN10 (34)    | PC4    | White  |
| Build-in     | PA5    | Green  |

