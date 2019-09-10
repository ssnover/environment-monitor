/*
 * Purpose: Interface to the thermometer application.
 */

#pragma once

#include <stdbool.h>

typedef enum 
{
    DEGREES_FAHRENHEIT,
    DEGREES_CELSIUS,
    DEGREES_KELVIN,
} DegreeScale;

typedef struct
{
  DegreeScale scale;
  uint16_t sample_period_milliseconds;
} ThermometerSettings;

void thermometer_task(void * parameters);

void thermometer_getSettings(ThermometerSettings const * settings);

void thermometer_updateSettings(ThermometerSettings const * const settings);
