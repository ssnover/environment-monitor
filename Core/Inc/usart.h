/*
 * Purpose: Interface to the USART driver.
 */

#pragma once

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32l4xx_hal.h"
#include "main.h"

extern void _Error_Handler(char *, int);

void uart_init();

uint8_t uart_write(uint8_t const * bytesToWrite, uint8_t numberOfBytes);

#ifdef __cplusplus
}
#endif
