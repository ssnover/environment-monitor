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
    task_settings.scale = DEGREES_KELVIN;
    task_settings.sample_period_milliseconds = 1000;

    ThermometerTaskParams * params = (ThermometerTaskParams *) parameters;

    TickType_t start_time = xTaskGetTickCount();
    while (1)
    {
        // Lock mutex
        volatile int16_t reading = getTemperatureFromRawADCValue(adc_read(), task_settings.scale);
	volatile BaseType_t res = xQueueSend(params->sensorDataQueue, (void *) &reading, pdMS_TO_TICKS(500));
        TickType_t delayTime = pdMS_TO_TICKS(task_settings.sample_period_milliseconds);
        // Unlock mutex
        vTaskDelayUntil(&start_time, delayTime);
    }
}

void thermometer_getSettings(ThermometerSettings const * settings)
{
}

void thermometer_updateSettings(ThermometerSettings const * const settings)
{
}

int16_t getTemperatureFromRawADCValue(uint32_t adcValue, DegreeScale scale)
{
    int16_t retVal = 0;
    float const VREF_MILLIVOLTS = 3300.f;
    float const MEASURED_MILLIVOLTS = VREF_MILLIVOLTS * ((float) adcValue) / ((float) (1 << 12u));
    float const MEASURED_DEGREES_CELSIUS = (MEASURED_MILLIVOLTS * (0.1)) - 50.;
    switch (scale)
    {
	case DEGREES_CELSIUS:
	    retVal = (int16_t) MEASURED_DEGREES_CELSIUS;
	    break;
        case DEGREES_KELVIN:
	    retVal = ((int16_t) MEASURED_DEGREES_CELSIUS) + 273;
	    break;
	case DEGREES_FAHRENHEIT:
	    retVal = (int16_t) ((MEASURED_DEGREES_CELSIUS * 1.8) + 32);
	    break;
        default:
            retVal = 0;
            break;
    }
    return retVal;
}
