/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : stm32f7xx_hal_msp.c
  * Description        : This file provides code for the MSP Initialization
  *                      and de-Initialization codes.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
  /* USER CODE END Header */

  /* Includes ------------------------------------------------------------------*/
#include "main.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */
extern DMA_HandleTypeDef hdma_spi2_rx;

extern DMA_HandleTypeDef hdma_spi3_tx;

extern DMA_HandleTypeDef hdma_adc1;

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN Define */

/* USER CODE END Define */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN Macro */

/* USER CODE END Macro */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* External functions --------------------------------------------------------*/
/* USER CODE BEGIN ExternalFunctions */

/* USER CODE END ExternalFunctions */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{
    /* USER CODE BEGIN MspInit 0 */

    /* USER CODE END MspInit 0 */

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_RCC_SYSCFG_CLK_ENABLE();

    /* System interrupt init*/

    /* USER CODE BEGIN MspInit 1 */

    /* USER CODE END MspInit 1 */
}

/**
* @brief I2C MSP Initialization
* This function configures the hardware resources used in this example
* @param hi2c: I2C handle pointer
* @retval None
*/
void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    if (hi2c->Instance == I2C2)
    {
        /* USER CODE BEGIN I2C2_MspInit 0 */

        /* USER CODE END I2C2_MspInit 0 */

        __HAL_RCC_GPIOF_CLK_ENABLE();
        /**I2C2 GPIO Configuration
        PF0     ------> I2C2_SDA
        PF1     ------> I2C2_SCL
        */
        GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
        HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

        /* Peripheral clock enable */
        __HAL_RCC_I2C2_CLK_ENABLE();
        /* USER CODE BEGIN I2C2_MspInit 1 */

        /* USER CODE END I2C2_MspInit 1 */
    }

}

/**
* @brief ADC MSP Initialization
* This function configures the hardware resources used in this example
* @param hadc: ADC handle pointer
* @retval None
*/
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    if (hadc->Instance == ADC1)
    {
        /* USER CODE BEGIN ADC1_MspInit 0 */

        /* USER CODE END ADC1_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_ADC1_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**
         * ADC1 GPIO Configuration
            PC2     ------> ADC1_IN12
            PB1     ------> ADC1_IN9
        */

        GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_5 | GPIO_PIN_6;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_2;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);


        /* ADC1 DMA Init */
        /* ADC1 Init */
        hdma_adc1.Instance = DMA2_Stream0;
        hdma_adc1.Init.Channel = DMA_CHANNEL_0;
        hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
        hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
        hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
        hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
        hdma_adc1.Init.Mode = DMA_CIRCULAR;
        hdma_adc1.Init.Priority = DMA_PRIORITY_LOW;
        hdma_adc1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
        if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
        {
            Error_Handler();
        }

        __HAL_LINKDMA(hadc, DMA_Handle, hdma_adc1);

        /* ADC1 interrupt Init */
        HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(ADC_IRQn);
        /* USER CODE BEGIN ADC1_MspInit 1 */

        /* USER CODE END ADC1_MspInit 1 */
    }
}

/**
* @brief ADC MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hadc: ADC handle pointer
* @retval None
*/
void HAL_ADC_MspDeInit(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance == ADC1)
    {
        /* USER CODE BEGIN ADC1_MspDeInit 0 */

        /* USER CODE END ADC1_MspDeInit 0 */
         /* Peripheral clock disable */
        __HAL_RCC_ADC1_CLK_DISABLE();


        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_3);
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5);
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_6);
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_0);
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_1);
        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_0);
        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_2);

        /* ADC1 DMA DeInit */
        HAL_DMA_DeInit(hadc->DMA_Handle);

        /* ADC1 interrupt DeInit */
        HAL_NVIC_DisableIRQ(ADC_IRQn);
        /* USER CODE BEGIN ADC1_MspDeInit 1 */

        /* USER CODE END ADC1_MspDeInit 1 */
    }
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim_base)
{

    if (htim_base->Instance == TIM6)
    {
        /* USER CODE BEGIN TIM6_MspInit 0 */

        /* USER CODE END TIM6_MspInit 0 */
          /* Peripheral clock enable */
        __HAL_RCC_TIM6_CLK_ENABLE();

        /* TIM6 interrupt Init */
        HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);

        /* USER CODE BEGIN TIM6_MspInit 1 */

        /* USER CODE END TIM6_MspInit 1 */
    }

}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *htim_base)
{

    if (htim_base->Instance == TIM6)
    {
        /* USER CODE BEGIN TIM6_MspDeInit 0 */

        /* USER CODE END TIM6_MspDeInit 0 */
          /* Peripheral clock disable */
        __HAL_RCC_TIM6_CLK_DISABLE();

        /* TIM6 interrupt DeInit */
        HAL_NVIC_DisableIRQ(TIM6_DAC_IRQn);

        /* USER CODE BEGIN TIM6_MspDeInit 1 */

        /* USER CODE END TIM6_MspDeInit 1 */
    }

}

/**
* @brief I2C MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hi2c: I2C handle pointer
* @retval None
*/
void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance == I2C2)
    {
        /* USER CODE BEGIN I2C2_MspDeInit 0 */

        /* USER CODE END I2C2_MspDeInit 0 */
          /* Peripheral clock disable */
        __HAL_RCC_I2C2_CLK_DISABLE();

        /**I2C2 GPIO Configuration
        PF0     ------> I2C2_SDA
        PF1     ------> I2C2_SCL
        */
        HAL_GPIO_DeInit(GPIOF, GPIO_PIN_0 | GPIO_PIN_1);

        /* USER CODE BEGIN I2C2_MspDeInit 1 */

        /* USER CODE END I2C2_MspDeInit 1 */
    }

}

/**
* @brief I2S MSP Initialization
* This function configures the hardware resources used in this example
* @param hi2s: I2S handle pointer
* @retval None
*/
void HAL_I2S_MspInit(I2S_HandleTypeDef *hi2s)
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    if (hi2s->Instance == SPI2)
    {
        /* USER CODE BEGIN SPI2_MspInit 0 */

        /* USER CODE END SPI2_MspInit 0 */
          /* Peripheral clock enable */
        __HAL_RCC_SPI2_CLK_ENABLE();

        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**I2S2 GPIO Configuration
        PC3     ------> I2S2_SD
        PB10     ------> I2S2_CK
        PB12     ------> I2S2_WS
        PC6     ------> I2S2_MCK
        */
        GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_6;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_12;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* I2S2 DMA Init */
        /* SPI2_RX Init */
        hdma_spi2_rx.Instance = DMA1_Stream1;
#if defined (DMA_CHANNEL_9)
        hdma_spi2_rx.Init.Channel = DMA_CHANNEL_9;
#else
        hdma_spi2_rx.Init.Channel = DMA_CHANNEL_7;
#endif
        hdma_spi2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
        hdma_spi2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_spi2_rx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_spi2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hdma_spi2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
        hdma_spi2_rx.Init.Mode = DMA_CIRCULAR;
        hdma_spi2_rx.Init.Priority = DMA_PRIORITY_LOW;
        hdma_spi2_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
        if (HAL_DMA_Init(&hdma_spi2_rx) != HAL_OK)
        {
            Error_Handler();
        }

        __HAL_LINKDMA(hi2s, hdmarx, hdma_spi2_rx);

        /* USER CODE BEGIN SPI2_MspInit 1 */

        /* USER CODE END SPI2_MspInit 1 */
    }
    else if (hi2s->Instance == SPI3)
    {
        /* USER CODE BEGIN SPI3_MspInit 0 */

        /* USER CODE END SPI3_MspInit 0 */
          /* Peripheral clock enable */
        __HAL_RCC_SPI3_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        /**I2S3 GPIO Configuration
        PA4     ------> I2S3_WS
        PC10     ------> I2S3_CK
        PC12     ------> I2S3_SD
        */
        GPIO_InitStruct.Pin = GPIO_PIN_4;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_12;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        /* I2S3 DMA Init */
        /* SPI3_TX Init */
        hdma_spi3_tx.Instance = DMA1_Stream5;
        hdma_spi3_tx.Init.Channel = DMA_CHANNEL_0;
        hdma_spi3_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
        hdma_spi3_tx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_spi3_tx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_spi3_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hdma_spi3_tx.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
        hdma_spi3_tx.Init.Mode = DMA_CIRCULAR;
        hdma_spi3_tx.Init.Priority = DMA_PRIORITY_LOW;
        hdma_spi3_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
        if (HAL_DMA_Init(&hdma_spi3_tx) != HAL_OK)
        {
            Error_Handler();
        }

        __HAL_LINKDMA(hi2s, hdmatx, hdma_spi3_tx);

        /* USER CODE BEGIN SPI3_MspInit 1 */

        /* USER CODE END SPI3_MspInit 1 */
    }

}

/**
* @brief I2S MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hi2s: I2S handle pointer
* @retval None
*/
void HAL_I2S_MspDeInit(I2S_HandleTypeDef *hi2s)
{
    if (hi2s->Instance == SPI2)
    {
        /* USER CODE BEGIN SPI2_MspDeInit 0 */

        /* USER CODE END SPI2_MspDeInit 0 */
          /* Peripheral clock disable */
        __HAL_RCC_SPI2_CLK_DISABLE();

        /**I2S2 GPIO Configuration
        PC3     ------> I2S2_SD
        PB10     ------> I2S2_CK
        PB12     ------> I2S2_WS
        PC6     ------> I2S2_MCK
        */
        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_3 | GPIO_PIN_6);

        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10 | GPIO_PIN_12);

        /* I2S2 DMA DeInit */
        HAL_DMA_DeInit(hi2s->hdmarx);
        /* USER CODE BEGIN SPI2_MspDeInit 1 */

        /* USER CODE END SPI2_MspDeInit 1 */
    }
    else if (hi2s->Instance == SPI3)
    {
        /* USER CODE BEGIN SPI3_MspDeInit 0 */

        /* USER CODE END SPI3_MspDeInit 0 */
          /* Peripheral clock disable */
        __HAL_RCC_SPI3_CLK_DISABLE();

        /**I2S3 GPIO Configuration
        PA4     ------> I2S3_WS
        PC10     ------> I2S3_CK
        PC12     ------> I2S3_SD
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4);

        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_10 | GPIO_PIN_12);

        /* I2S3 DMA DeInit */
        HAL_DMA_DeInit(hi2s->hdmatx);
        /* USER CODE BEGIN SPI3_MspDeInit 1 */

        /* USER CODE END SPI3_MspDeInit 1 */
    }

}

/**
* @brief UART MSP Initialization
* This function configures the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    if (huart->Instance == USART3)
    {
        /* USER CODE BEGIN USART3_MspInit 0 */

        /* USER CODE END USART3_MspInit 0 */
          /* Peripheral clock enable */
        __HAL_RCC_USART3_CLK_ENABLE();

        __HAL_RCC_GPIOD_CLK_ENABLE();
        /**USART3 GPIO Configuration
        PD8     ------> USART3_TX
        PD9     ------> USART3_RX
        */
        GPIO_InitStruct.Pin = STLK_RX_Pin | STLK_TX_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

        /* USER CODE BEGIN USART3_MspInit 1 */

        /* USER CODE END USART3_MspInit 1 */
    }

}

/**
* @brief UART MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART3)
    {
        /* USER CODE BEGIN USART3_MspDeInit 0 */

        /* USER CODE END USART3_MspDeInit 0 */
          /* Peripheral clock disable */
        __HAL_RCC_USART3_CLK_DISABLE();

        /**USART3 GPIO Configuration
        PD8     ------> USART3_TX
        PD9     ------> USART3_RX
        */
        HAL_GPIO_DeInit(GPIOD, STLK_RX_Pin | STLK_TX_Pin);

        /* USER CODE BEGIN USART3_MspDeInit 1 */

        /* USER CODE END USART3_MspDeInit 1 */
    }

}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
