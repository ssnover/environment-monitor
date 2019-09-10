/*
 * Purpose: Defines an inteface to the gpio pins.
 */

#pragma once

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>
#include "stm32l4xx_hal.h"
#include "main.h"

typedef enum GPIO_State
{
    STATE_OFF = 0u,
    STATE_ON = 1u,
} GPIO_State;

void gpio_init();

void gpio_set(uint8_t pinNumber, GPIO_State state);

#ifdef __cplusplus
}
#endif
