/*
 * Implements access to the microcontroller ADC.
 */

#include "adc.h"
#include "gpio.h"

static ADC_HandleTypeDef PERIPHERAL_ADC1;

void adc_init(void)
{
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_ADC_CLK_ENABLE();
  GPIO_InitTypeDef gpio_init;
  gpio_init.Pin = GPIO_PIN_2;
  gpio_init.Mode = GPIO_MODE_ANALOG;
  gpio_init.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &gpio_init);

  PERIPHERAL_ADC1.Instance = ADC1;
  PERIPHERAL_ADC1.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
  PERIPHERAL_ADC1.Init.Resolution = ADC_RESOLUTION_12B;
  PERIPHERAL_ADC1.Init.ScanConvMode = DISABLE;
  PERIPHERAL_ADC1.Init.ContinuousConvMode = ENABLE;
  PERIPHERAL_ADC1.Init.DiscontinuousConvMode = DISABLE;
  PERIPHERAL_ADC1.Init.NbrOfDiscConversion = 0;
  PERIPHERAL_ADC1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  PERIPHERAL_ADC1.Init.ExternalTrigConv = ADC_EXTERNALTRIG_T1_CC1;
  PERIPHERAL_ADC1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  PERIPHERAL_ADC1.Init.NbrOfConversion = 1;
  PERIPHERAL_ADC1.Init.DMAContinuousRequests = ENABLE;
  PERIPHERAL_ADC1.Init.EOCSelection = DISABLE;
  HAL_ADC_Init(&PERIPHERAL_ADC1);

  ADC_ChannelConfTypeDef adc_channel_config;
  adc_channel_config.Channel = ADC_CHANNEL_3;
  adc_channel_config.Rank = 1;
  adc_channel_config.SamplingTime = ADC_SAMPLETIME_640CYCLES_5;
  adc_channel_config.Offset = 0;

  if (HAL_ADC_ConfigChannel(&PERIPHERAL_ADC1, &adc_channel_config) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  HAL_ADC_Start(&PERIPHERAL_ADC1);
}

uint32_t adc_read()
{
    uint32_t retVal = 0u;
    if (HAL_OK == HAL_ADC_PollForConversion(&PERIPHERAL_ADC1, 1000000))
    {
        retVal = HAL_ADC_GetValue(&PERIPHERAL_ADC1);
    }
    return retVal;
}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(adcHandle->Instance==ADC1)
  {
    /* ADC1 clock enable */
    __HAL_RCC_ADC_CLK_ENABLE();
  
    GPIO_InitStruct.Pin = ARD_A5_Pin|ARD_A4_Pin|ARD_A3_Pin|ARD_A2_Pin 
                          |ARD_A1_Pin|ARD_A0_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = ARD_D7_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(ARD_D7_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = ARD_D6_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(ARD_D6_GPIO_Port, &GPIO_InitStruct);
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{
  if (adcHandle->Instance == ADC1)
  {
    /* Peripheral clock disable */
    __HAL_RCC_ADC_CLK_DISABLE();
    HAL_GPIO_DeInit(GPIOC, ARD_A5_Pin|ARD_A4_Pin|ARD_A3_Pin|ARD_A2_Pin 
                          |ARD_A1_Pin|ARD_A0_Pin);
    HAL_GPIO_DeInit(ARD_D7_GPIO_Port, ARD_D7_Pin);
    HAL_GPIO_DeInit(ARD_D6_GPIO_Port, ARD_D6_Pin);
  }
} 
