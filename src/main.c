/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "codec/CS43L22.h"
#include "audioI2S.h"
#include <stdlib.h>
#include "uexkull.h"
#include <math.h>

#define PI 3.14159f

#define F_SAMPLE 48000.0f
#define F_OUT 440.0f

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

I2S_HandleTypeDef hi2s3;
DMA_HandleTypeDef hdma_spi3_tx;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2S3_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
float mySinVal;
float sample_dt;
uint16_t sample_N;
uint16_t i_t;

static uint16_t audioBuffer[I2S_BUFFER_SIZE];

uint32_t myDacVal;
static __IO uint32_t audioRemainSize = 0;
static uint32_t fileLength = I2S_BUFFER_SIZE;
static uint32_t playerReadBytes = 0;

typedef enum
{
    PLAYER_CONTROL_Idle,
    PLAYER_CONTROL_HalfBuffer,
    PLAYER_CONTROL_FullBuffer,
} PLAYER_CONTROL_e;

static volatile PLAYER_CONTROL_e playerControlSM = PLAYER_CONTROL_Idle;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    /* USER CODE BEGIN 1 */
    sample_dt = F_OUT / F_SAMPLE;
    sample_N = F_SAMPLE / F_OUT;
    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_I2C1_Init();
    MX_I2S3_Init();

    UX_init(&uexkull, 48000);

    /* USER CODE BEGIN 2 */
    CS43_Init(hi2c1, MODE_I2S);
    CS43_SetVolume(0);
    CS43_Enable_RightLeft(CS43_RIGHT_LEFT);
    audioI2S_setHandle(&hi2s3);

    // for (uint16_t i = 0; i < 16; i++)
    // {
    //     // mySinVal = UX_process(&uexkull, 0.5, 440);
    //     mySinVal = sinf(i * 2 * PI * (440.0f / 48000.0f));
    //     dataI2S[i * 2] = (mySinVal + 1) * 8192;
    //     dataI2S[i * 2 + 1] = (mySinVal + 1) * 8192;
    // }

    audioI2S_setHandle(&hi2s3);
    audioI2S_init(48000);
    audioRemainSize = fileLength - playerReadBytes;
    audioI2S_play((uint16_t *)&audioBuffer[0], I2S_BUFFER_SIZE);

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        switch (playerControlSM)
        {
        case PLAYER_CONTROL_Idle:
            break;

        case PLAYER_CONTROL_HalfBuffer:
            playerReadBytes = 0;
            playerControlSM = PLAYER_CONTROL_Idle;
            // TODO: Translate f_read function into UX_process() -> audioBuffer
            // f_read(&wavFile, &audioBuffer[0], I2S_BUFFER_SIZE / 2, &playerReadBytes);
            for (int i = 0; i < I2S_BUFFER_SIZE / 4; i++)
            {
                audioBuffer[i * 2] = (sinf(i * 2 * PI * (440.0f / 48000.0f)) + 1.0f) * 8192.0f;
                audioBuffer[i * 2 + 1] = (sinf(i * 2 * PI * (440.0f / 48000.0f)) + 1.0f) * 8192.0f;

                playerReadBytes++;
                if (audioRemainSize > (I2S_BUFFER_SIZE / 2))
                {
                    audioRemainSize -= playerReadBytes;
                }
                else
                {
                    audioRemainSize = 0;
                    playerReadBytes = 0;
                }
            }

            break;

        case PLAYER_CONTROL_FullBuffer:
            playerReadBytes = 0;
            playerControlSM = PLAYER_CONTROL_Idle;
            // TODO: Translate f_read function into UX_process() -> audioBuffer
            // f_read(&wavFile, &audioBuffer[I2S_BUFFER_SIZE / 2], I2S_BUFFER_SIZE / 2, &playerReadBytes);
            for (int i = 0; i < I2S_BUFFER_SIZE / 4; i++)
            {
                // audioBuffer[(I2S_BUFFER_SIZE / 4) + i * 2] = (sinf(i * 2 * PI * (440.0f / 48000.0f)) + 1.0f) * 8192.0f;
                // audioBuffer[(I2S_BUFFER_SIZE / 4) + i * 2 + 1] = (sinf(i * 2 * PI * (440.0f / 48000.0f)) + 1.0f) * 8192.0f;

                playerReadBytes++;
                if (audioRemainSize > (I2S_BUFFER_SIZE / 2))
                {
                    audioRemainSize -= playerReadBytes;
                }
                else
                {
                    audioRemainSize = 0;
                    playerReadBytes = 0;
                }
            }
            break;
        }
    }

    audioI2S_stop();

    /* USER CODE END 3 */
}

/**
 * @brief Half/Full transfer Audio callback for buffer management
 */
void audioI2S_halfTransfer_Callback(void)
{
    playerControlSM = PLAYER_CONTROL_HalfBuffer;
}
void audioI2S_fullTransfer_Callback(void)
{
    playerControlSM = PLAYER_CONTROL_FullBuffer;
    audioI2S_changeBuffer((uint16_t *)&audioBuffer[0], I2S_BUFFER_SIZE / 2);
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
  */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 4;
    RCC_OscInitStruct.PLL.PLLN = 168;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB buses clocks
  */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
    {
        Error_Handler();
    }
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2S;
    PeriphClkInitStruct.PLLI2S.PLLI2SN = 50;
    PeriphClkInitStruct.PLLI2S.PLLI2SR = 2;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
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

/**
  * @brief I2S3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2S3_Init(void)
{

    /* USER CODE BEGIN I2S3_Init 0 */

    /* USER CODE END I2S3_Init 0 */

    /* USER CODE BEGIN I2S3_Init 1 */

    /* USER CODE END I2S3_Init 1 */
    hi2s3.Instance = SPI3;
    hi2s3.Init.Mode = I2S_MODE_MASTER_TX;
    hi2s3.Init.Standard = I2S_STANDARD_PHILIPS;
    hi2s3.Init.DataFormat = I2S_DATAFORMAT_16B;
    hi2s3.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
    hi2s3.Init.AudioFreq = I2S_AUDIOFREQ_48K;
    hi2s3.Init.CPOL = I2S_CPOL_LOW;
    hi2s3.Init.ClockSource = I2S_CLOCK_PLL;
    hi2s3.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;
    if (HAL_I2S_Init(&hi2s3) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN I2S3_Init 2 */

    /* USER CODE END I2S3_Init 2 */
}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

    /* DMA controller clock enable */
    __HAL_RCC_DMA1_CLK_ENABLE();

    /* DMA interrupt init */
    /* DMA1_Stream5_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_4, GPIO_PIN_RESET);

    /*Configure GPIO pins : PD12 PD13 PD14 PD15
                           PD4 */
    GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}

/* USER CODE BEGIN 4 */
/*void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	UNUSED(htim);
	
	if(htim->Instance == TIM2){
		mySinVal = sinf(i_t * 2 * PI * sample_dt);
		
		myDacVal = (mySinVal + 1) * 127;
		
		HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_8B_R, myDacVal);
		
		i_t++;
		if(i_t >= sample_N) i_t = 0;
	}
}*/
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
