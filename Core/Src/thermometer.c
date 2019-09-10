/*
 * Purpose: Implement the thermometer task and functions for interacting with it.
 */

#include <FreeRTOS.h>
#include <task.h>
#include "thermometer.h"
#include "adc.h"

static int16_t getTemperatureFromRawADCValue(uint32_t adcValue, DegreeScale scale);

static ThermometerSettings task_settings;

void thermometer_task(void * parameters)
{
    // Lock this with a mutex
    task_settings.scale = DEGREES_FAHRENHEIT;
    task_settings.sample_period_milliseconds = 1000;

    TickType_t start_time = xTaskGetTickCount();
    while (1)
    {
        // Lock mutex
        volatile int16_t reading = getTemperatureFromRawADCValue(adc_read(), task_settings.scale);
        TickType_t delayTime = pdMS_TO_TICKS(task_settings.sample_period_milliseconds);
        // Unlock mutex
        vTaskDelayUntil(&start_time, delayTime);
    }
}

void thermometer_getSettings(ThermometerSettings const * settings)
{
    // Should probably lock and copy data, not just pass pointer
    settings = &task_settings;
}

void thermometer_updateSettings(ThermometerSettings const * const settings)
{
    // Lock mutex
    task_settings = *settings;
    // Unlock mutex
}

int16_t getTemperatureFromRawADCValue(uint32_t adcValue, DegreeScale scale)
{
    int16_t retVal = 0;
    switch (scale)
    {
        case DEGREES_FAHRENHEIT:
            retVal = 98;
            break;
        default:
            retVal = 0;
            break;
    }
}
