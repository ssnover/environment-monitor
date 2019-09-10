/*
 * Purpose: Implementation of the heartbeat LED driver.
 */

#include <FreeRTOS.h>
#include <task.h>
#include "heartbeat-led.h"

void heartbeat_led_task(void * parameters)
{
    TickType_t const HEARTBEAT_PERIOD_TICKS = pdMS_TO_TICKS(2000u);
    TickType_t start_time = xTaskGetTickCount();
    while (1)
    {
        gpio_set(0, STATE_ON);
        vTaskDelayUntil(&start_time, HEARTBEAT_PERIOD_TICKS / 2);
        gpio_set(0, STATE_OFF);
        vTaskDelayUntil(&start_time, HEARTBEAT_PERIOD_TICKS / 2);
    }
}
