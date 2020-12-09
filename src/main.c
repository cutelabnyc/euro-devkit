/*
 * This is a blink example using the stm32f4-template (https://github.com/sferrini/stm32f4-template)
 */

#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_rcc.h>
#include <stm32f4xx_hal_gpio.h>
#include <system_stm32f4xx.h>

UART_HandleTypeDef huart2;
void SystemClock_Config(void);
static void MX_USART2_UART_Init(void);

int main(void)
{
    HAL_Init();
    //HAL_RCC_ClockConfig();

    GPIO_InitTypeDef GPIO_InitStruct;
    __GPIOC_CLK_ENABLE();

    /*Configure GPIO pin : PC12 */
    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD; // digital Input
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_12);
        HAL_Delay(1000);
    }

    return 0; // never returns actually
}