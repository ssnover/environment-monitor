/*
 * Purpose: Interface to the data stream output.
 */

#pragma once

#include <queue.h>

typedef struct
{
  QueueHandle_t sensorDataQueue;
} OutputTaskParams;

void output_task(void * parameters);

