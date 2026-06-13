/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2c.c
  * @brief   This file provides code for the configuration
  *          of the I2C instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "i2c.h"
#include "usart.h"
#include <stdlib.h>

/* USER CODE BEGIN 0 */

uint8_t i2c_tx_buffer[TX_MAX_BUFFER_SIZE] = {0};
uint8_t i2c_rx_buffer[RX_MAX_BUFFER_SIZE] = {0};

/* USER CODE END 0 */

I2C_HandleTypeDef hi2c1;
DMA_HandleTypeDef hdma_i2c1_rx;

// TODO: Implement DMA RX - possibly use boolean "i2c_rx_received"
bool rx_received = false; // flag that gets set when I2C RX is trigged

/* I2C1 init function */
void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspInit 0 */

  /* USER CODE END I2C1_MspInit 0 */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C1 GPIO Configuration
    PB8     ------> I2C1_SCL
    PB9     ------> I2C1_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* I2C1 clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();

    /* I2C1 DMA Init */
    /* I2C1_RX Init */
    hdma_i2c1_rx.Instance = DMA1_Stream0;
    hdma_i2c1_rx.Init.Channel = DMA_CHANNEL_1;
    hdma_i2c1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_i2c1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_i2c1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_i2c1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_i2c1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_i2c1_rx.Init.Mode = DMA_CIRCULAR;
    hdma_i2c1_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_i2c1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_i2c1_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(i2cHandle,hdmarx,hdma_i2c1_rx);

    /* I2C1 interrupt Init */
    HAL_NVIC_SetPriority(I2C1_EV_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
  /* USER CODE BEGIN I2C1_MspInit 1 */

  /* USER CODE END I2C1_MspInit 1 */
  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspDeInit 0 */

  /* USER CODE END I2C1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C1_CLK_DISABLE();

    /**I2C1 GPIO Configuration
    PB8     ------> I2C1_SCL
    PB9     ------> I2C1_SDA
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_9);

    /* I2C1 DMA DeInit */
    HAL_DMA_DeInit(i2cHandle->hdmarx);

    /* I2C1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(I2C1_EV_IRQn);
  /* USER CODE BEGIN I2C1_MspDeInit 1 */

  /* USER CODE END I2C1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/**
 * @brief Override the Rx Complete Callback for non-blocking DMA
 * @param[in] hi2cHandle - handle for I2C instance
 * TODO: Implement DMA RX - possibly use boolean "i2c_rx_received"
 */
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2cHandle)
{
	if(hi2cHandle->Instance == I2C1)
	{
		// Data is now in the buffer

	}
}

/**
 * @brief Scan all 128 addresses to check which are populated
 * @param None
 * @site: https://www.youtube.com/watch?v=n7vlq_67FI0
 * @site: https://stm32world.com/wiki/Category:STM32_Development
 * @site: https://stm32world.com/wiki/STM32_Scan_I%C2%B2C_bus
 */
void i2c_scan(void)
{

	for(uint8_t i = 0; i < 128; i++)
	{
		// Check if device is available
		if(I2C_IsDeviceReady(i))
		{
			// Device is available - We got an ACK
			printf("0x%2X\r\n", i);
		}
		else
		{
			// Device is not available
			//printf("---- ");
		}

		// Format the line to 16 characters per line
		//if(i > 0 && (i+1) % 16 == 0) printf("\r\n");

	}
}

/**
 * @brief Wrapper function for HAL_I2C_IsDeviceReady
 * @param[in] address - The address of the I2C peripheral to check
 * @return[out] True: The device is ready, False: Device is not ready
 */
bool I2C_IsDeviceReady(uint8_t address)
{
	bool ready = false;
	uint8_t try_attempts = 3;
	uint8_t timeout = 5;

	if(HAL_I2C_IsDeviceReady(&hi2c1, (uint16_t)(address << 1), (uint32_t)try_attempts, (uint32_t)timeout ) == HAL_OK)
	{
		ready = true;
	}

	return ready;
}


/**
 * @brief HAL I2C1 Transmit wrapper
 * @param[in] address - Target address
 * @param[in] reg_addr - Address of register
 * @param[in] *data   - data to transmit (e.g. data[0] = 0x08)
 * @param[in] size    - size of the data to transmit (e.g. 1)
 */
void I2C_MasterTransmit(uint8_t address, uint8_t reg_addr, uint8_t* buffer, uint16_t size)
{
	//HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)
	//uint8_t buff[20] = {0};
	/*
	uint8_t* tx_buff = (uint8_t*)malloc((size+1)*sizeof(uint8_t));

	// register address should be sent first
	tx_buff[0] = reg_addr;

	// followed by the data to be sent
	for(uint16_t i = 0; i < size-1; i++)
	{
		// Append to buffer after index 0
		tx_buff[i+1] = buffer[i];
	}
	*/
	uint8_t* tx_buffer = I2C_CreateBuffer(reg_addr, buffer, size);

	if(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)(address<<1), tx_buffer, size, I2C_MAX_TIMEOUT) != HAL_OK)
	{
		Error_Handler();
	}

	// free the temp buffer
	free(tx_buffer);
	tx_buffer = NULL;
}

/**
 * @brief HAL I2C1 Receive wrapper
 * @param[in] address - Target address
 * @param[in] *data   - data to transmit (e.g. data[0] = 0x08)
 * @param[in] size    - size of the data to transmit (e.g. 1)
 */
void I2C_MasterReceive(uint8_t address, uint8_t* buffer, uint8_t size)
{
	//HAL_StatusTypeDef HAL_I2C_Master_Receive_DMA(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size)
	//i2c_rx_buffer

	/*
	if(HAL_I2C_Master_Receive_DMA(&hi2c1, (uint16_t)(address<<1), i2c_rx_buffer, size) != HAL_OK)
	{
		Error_Handler();
	}
	*/

	//HAL_StatusTypeDef HAL_I2C_Master_Receive(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout);

	// Read from I2C - this gets stored into the rx_buffer
	if(HAL_I2C_Master_Receive(&hi2c1, (uint16_t)(address<<1), buffer, size, I2C_MAX_TIMEOUT) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief Dynamically create a buffer to combine both the address and the original buffer
 * @param[in] reg_addr - target register address
 * @param[in] *buffer - the buffer to be sent
 * @param[in] size - Original amount of data in the buffer
 */
uint8_t *I2C_CreateBuffer(uint8_t reg_addr, uint8_t* buffer, uint32_t size)
{
	uint8_t* out_buffer = (uint8_t*)malloc((size+1)*sizeof(uint8_t));

	// register address should be sent first
	out_buffer[0] = reg_addr;

	// followed by the data to be sent
	for(uint16_t i = 0; i < size-1; i++)
	{
		// Append to buffer after index 0
		out_buffer[i+1] = buffer[i];
	}

	return out_buffer;
}

/**
 * @brief Perform I2C Read
 * @param[in] address - Target device address
 * @param[in] reg_addr - Target register to read
 * @param[out] *buffer - buffer array to store results
 * @param[in] size - amount of data to be received
 */
bool I2C_Read(uint8_t address, uint8_t reg_addr, uint8_t* buffer, uint16_t size)
{
	bool result = false;

	//HAL_StatusTypeDef HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)

	if(HAL_I2C_Mem_Read(&hi2c1, (uint16_t)(address<<1), (uint16_t)reg_addr, I2C_MEMADD_SIZE_8BIT, buffer, size, I2C_MAX_TIMEOUT) != HAL_OK)
	{
		// Not OK - an error occurre
		printf("> I2C Mem Read - ERROR");
	}
	else
	{
		// HAL_OK - success
		result = true;
	}

	return result;
}

/**
 * @brief Perform I2C Read
 * @param[in] address - Target device address
 * @param[in] reg_addr - Target register to read
 * @param[out] *buffer - buffer array to store results
 * @param[in] size - amount of data to be sent
 */
bool I2C_Write(uint8_t address, uint8_t reg_addr, uint8_t* buffer, uint16_t size)
{
	bool result = false;
	//HAL_StatusTypeDef HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)

	if(HAL_I2C_Mem_Write(&hi2c1, (uint16_t)(address<<1), reg_addr, I2C_MEMADD_SIZE_8BIT, buffer, size, I2C_MAX_TIMEOUT) != HAL_OK)
	{
		// Not OK - an error occurred

		printf("> I2C Mem Write - ERROR");
	}
	else
	{
		// HAL_OK - success
		result = true;
	}

	return result;
}

/* USER CODE END 1 */

