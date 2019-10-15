/*
 * Purpose: Implementation of USART driver.
 */

#include "usart.h"
#include "gpio.h"

#define BUFFER_SIZE (100)

static UART_HandleTypeDef PERIPHERAL_UART4;
static uint8_t UART_WRITE_BUFFER[BUFFER_SIZE];
static uint8_t write_buffer_head;
static uint8_t write_buffer_tail;
static uint8_t UART_READ_BUFFER[BUFFER_SIZE];
static uint8_t read_buffer_head;
static uint8_t read_buffer_tail;

/*
 * Writes the next byte in the ring buffer into the hardware buffer.
 */
void uart_sendNextByte();

extern void UART4_IRQHandler()
{
  HAL_UART_IRQHandler(&PERIPHERAL_UART4);
}

void uart_init()
{
  __HAL_RCC_UART4_CLK_ENABLE();
  __GPIOA_CLK_ENABLE();

  GPIO_InitTypeDef gpioA;
  gpioA.Pin = GPIO_PIN_0;
  gpioA.Mode = GPIO_MODE_AF_PP;
  gpioA.Alternate = GPIO_AF8_UART4;
  gpioA.Speed = GPIO_SPEED_HIGH;
  gpioA.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &gpioA);
  gpioA.Pin = GPIO_PIN_1;
  gpioA.Mode = GPIO_MODE_AF_OD;
  HAL_GPIO_Init(GPIOA, &gpioA);

  PERIPHERAL_UART4.Instance = UART4;
  PERIPHERAL_UART4.Init.BaudRate = 19200;
  PERIPHERAL_UART4.Init.WordLength = UART_WORDLENGTH_8B;
  PERIPHERAL_UART4.Init.StopBits = UART_STOPBITS_1;
  PERIPHERAL_UART4.Init.Parity = UART_PARITY_NONE;
  PERIPHERAL_UART4.Init.Mode = UART_MODE_TX_RX;
  PERIPHERAL_UART4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  PERIPHERAL_UART4.Init.OverSampling = UART_OVERSAMPLING_16;
  PERIPHERAL_UART4.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  PERIPHERAL_UART4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&PERIPHERAL_UART4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  write_buffer_head = 0;
  write_buffer_tail = 0;

  NVIC_EnableIRQ(UART4_IRQn);
}

uint8_t uart_write(uint8_t const * bytesToWrite, uint8_t numberOfBytes)
{
  for (uint8_t byteIndex = 0u; byteIndex < numberOfBytes; ++byteIndex)
  {
    UART_WRITE_BUFFER[write_buffer_head++] = bytesToWrite[byteIndex];
    if (write_buffer_head >= BUFFER_SIZE)
    {
      write_buffer_head = 0;
    }
  }
  uart_sendNextByte();
  return 0;
}

uint8_t uart_read(uint8_t * readBuffer, uint8_t numberOfBytes)
{
  uint8_t byteIndex = 0u;
  for (byteIndex; 
       (byteIndex < numberOfBytes) && (read_buffer_head != read_buffer_tail); 
       ++byteIndex)
  {
    readBuffer[byteIndex] = UART_READ_BUFFER[read_buffer_tail++];
    if (read_buffer_tail >= BUFFER_SIZE)
    {
      read_buffer_tail = 0;
    }
  }
  return byteIndex;
}

void uart_sendNextByte()
{
  HAL_UART_Transmit_IT(&PERIPHERAL_UART4, UART_WRITE_BUFFER + write_buffer_tail, 1);
  write_buffer_tail++;
  if (write_buffer_tail >= BUFFER_SIZE)
  {
    write_buffer_tail = 0;
  }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef * handle)
{
  if (write_buffer_tail != write_buffer_head)
  {
    uart_sendNextByte();
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef * handle)
{
  HAL_UART_Receive_IT(&PERIPHERAL_UART4, UART_READ_BUFFER + read_buffer_head, 1);
  read_buffer_head++;
  if (read_buffer_head >= BUFFER_SIZE)
  {
    read_buffer_head = 0;
  }
}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();
  
    /**USART1 GPIO Configuration    
    PB6     ------> USART1_TX
    PB7     ------> USART1_RX 
    */
    GPIO_InitStruct.Pin = ST_LINK_UART1_TX_Pin|ST_LINK_UART1_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();
  
    /**USART1 GPIO Configuration    
    PB6     ------> USART1_TX
    PB7     ------> USART1_RX 
    */
    HAL_GPIO_DeInit(GPIOB, ST_LINK_UART1_TX_Pin|ST_LINK_UART1_RX_Pin);

  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
