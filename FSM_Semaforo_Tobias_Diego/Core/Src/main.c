/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
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
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>

#include "blink_control.h"
#include "timer_period_manager.h"
#include "timer.h"

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

/* USER CODE BEGIN PV */
// Low-power mode aids
Timer tick,sys_tick_2;
unsigned int bandera=0;

DebouncedSwitch debounced_button1, debounced_button2;
EdgeDetector edge_detector1, edge_detector2;
BlinkControl blink_control_led1, blink_control_led2;
TimerPeriodManager period_manager1, period_manager2;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

	/* USER CODE BEGIN 1 */

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
	/* USER CODE BEGIN 2 */
	debounced_switch_init(&debounced_button1, SW_1_GPIO_Port, SW_1_Pin);
	debounced_switch_init(&debounced_button2, SW_2_GPIO_Port, SW_2_Pin);
	blink_control_init(&blink_control_led1, LED_1_GPIO_Port, LED_1_Pin, 0);
	blink_control_init(&blink_control_led2, LED_2_GPIO_Port, LED_2_Pin, 0);
	edge_detector_init(&edge_detector1, &debounced_button1);
	edge_detector_init(&edge_detector2, &debounced_button2);
	timer_period_manager_init(&period_manager1, &blink_control_led1.blink_timer, &edge_detector1);
	timer_period_manager_init(&period_manager2, &blink_control_led2.blink_timer, &edge_detector2);
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{

		if (bandera==0)
		{
			//Led Rojo
			HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, 1);
			//Led Verde
			HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, 0);
		}
		if (bandera==0 && (HAL_GPIO_ReadPin(SW_1_GPIO_Port, SW_1_Pin) || HAL_GPIO_ReadPin(SW_2_GPIO_Port, SW_2_Pin)))
		{
			//Led Verde
			HAL_GPIO_TogglePin(LED_2_GPIO_Port, LED_2_Pin);
			timer_start(&tick, 200);
			bandera++;
		}
		if (bandera>0 && bandera<6 && timer_has_expired(&tick))
		{
			//Led Verde
			HAL_GPIO_TogglePin(LED_2_GPIO_Port, LED_2_Pin);
			timer_restart(&tick);
			bandera++;
		}
		if(bandera==6)
		{
			//Led Rojo
			HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, 0);
			//Led Verde
			HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, 1);
			timer_restart(&tick);
			timer_update_duration(&tick, 3000);
			bandera++;
		}
		if (bandera==7 && timer_has_expired(&tick))
		{
			//Led Rojo
			HAL_GPIO_TogglePin(LED_1_GPIO_Port, LED_1_Pin);

			timer_restart(&tick);
			timer_update_duration(&tick, 200);
			bandera++;
		}
		if(bandera>7 && bandera<12 && timer_has_expired(&tick))
		{
			//Led Rojo
			HAL_GPIO_TogglePin(LED_1_GPIO_Port, LED_1_Pin);
			timer_restart(&tick);
			bandera++;
		}
		if(bandera==12)
		{
			bandera=0;
		}


		// Put the processor to sleep during the remaining time until the tick rate is reached,
		// this allows us to save energy from otherwise wasted CPU cycles. The systick will wake up
		// the processor once every millisecond, and then be back to sleep again, this process will
		// repeat until the timer expires. Then our tasks will run again one time, and so on.
		while(!timer_has_expired(&tick)) {
			HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
		}
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/* USER CODE BEGIN MX_GPIO_Init_1 */
	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin : LED_1_Pin */
	GPIO_InitStruct.Pin = LED_1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LED_1_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : LED_2_Pin */
	GPIO_InitStruct.Pin = LED_2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LED_2_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : SW_1_Pin SW_2_Pin */
	GPIO_InitStruct.Pin = SW_1_Pin|SW_2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* USER CODE BEGIN MX_GPIO_Init_2 */
	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/**
 * @brief  Retargets the C library printf function to the USART.
 *   None
 * @retval None
 */

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

#ifdef  USE_FULL_ASSERT
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
