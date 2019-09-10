/*
 * Defines interface to analog-digital converter.
 */

#pragma once

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32l4xx_hal.h"
#include "main.h"

extern void _Error_Handler(char *, int);

void adc_init();

uint32_t adc_read();

#ifdef __cplusplus
}
#endif
