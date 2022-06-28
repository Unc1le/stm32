#include "main.h"

uint8_t mode = 0;
uint8_t led_on = 0;
//uint8_t delay = 100;
uint8_t on_step;
uint16_t angle = 0;
uint16_t count = 0;
//uint16_t count_sp = 0;
uint8_t step = 0;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	on_step = !on_step; count = 0;
}

int main(void) {
	HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();

	while (1) {

		if (on_step == 0 && count < angle*1.422) {
			step = step + 1;
			HAL_Delay(2);
			if (step > 3) {
				step = 0;
				count = count + 1;
			}

			switch (step) {
			case 0:
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, SET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, RESET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, RESET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, RESET);
				break;

			case 1:
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, RESET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, SET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, RESET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, RESET);
				break;

			case 2:
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, RESET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, RESET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, SET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, RESET);
				break;

			case 3:
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, RESET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, RESET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, RESET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, SET);
				break;
			}
		}

		if (on_step && count < angle*1.422) {

						step = step + 1;
						HAL_Delay(2);
						if (step > 3) {
							step = 0;
							count = count + 1; }

			switch (step) {
						case 0:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, RESET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, RESET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, RESET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, SET);
			break;

						case 1:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, RESET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, RESET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, SET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, RESET);
			break;

						case 2:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, RESET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, SET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, RESET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, RESET);
			break;

						case 3:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, SET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, RESET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, RESET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, RESET);
			break;}
		}
	}

}

void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_UART4;
	PeriphClkInit.Uart4ClockSelection = RCC_UART4CLKSOURCE_PCLK1;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}
}

static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	HAL_GPIO_WritePin(GPIOE,
			GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12
					| GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(GPIOD,
	GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11, GPIO_PIN_RESET);

	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11
			| GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);

}

void Error_Handler(void) {

	__disable_irq();
	while (1) {
	}

}

#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t *file, uint32_t line)
{

}
#endif
