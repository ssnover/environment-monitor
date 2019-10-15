/*
 * Purpose: Implement a task which prints out data to the world.
 */

#include <stdlib.h>
#include <string.h>
#include <FreeRTOS.h>
#include <task.h>
#include "data-output.h"
#include "usart.h"

static void printHelloWorld();
static uint8_t itoaBuffer[10];

void output_task(void * parameters)
{
  // Print then spin
  printHelloWorld();
  
  OutputTaskParams * params = (OutputTaskParams *) parameters;

  while (1)
  {
    uint16_t sensor_reading;
    if (pdPASS == xQueueReceive(params->sensorDataQueue, (void *) &sensor_reading, pdMS_TO_TICKS(500)))
    {
      // Got some data now let's send it!
      itoa(sensor_reading, itoaBuffer, 10);
      uart_write(itoaBuffer, strlen(itoaBuffer));
      uart_write("\r\n", 2);
    }
  }
}

void printHelloWorld()
{
  static uint8_t const HELLO_WORLD_STRING[] = "Hello, World!\0";
  uart_write(HELLO_WORLD_STRING, 13);
}

