#include "main.h"

uint8_t mode = 0;
uint8_t led_on = 0;
//uint8_t delay = 100;
uint8_t rxdata[5] = { 0 };
uint8_t txdata[5] = { 0xff };
uint8_t TIM_count = 0;
uint8_t on_step;

typedef enum {
 STEP_0 = 1 << 0,
 STEP_1 = 1 << 1,
 STEP_2 = 1 << 2,
 STEP_3 = 1 << 3,
} motor_step;

typedef enum {
  CLOCKWISE,
  COUNTER_CLOCKWISE,
} rotation_t;
static rotation_t rotation = CLOCKWISE;

UART_HandleTypeDef huart4;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
 on_step = !on_step;

 rotation = rotation == CLOCKWISE
     ? COUNTER_CLOCKWISE
     : CLOCKWISE;

 }

static void rotate_clockwise(motor_step steps) {
	HAL_Delay(2);
	  	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, steps & (STEP_0));
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, steps & (STEP_1));
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, steps & (STEP_2));
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, steps & (STEP_3));
}

static void rotate_counter_clockwise(motor_step steps) {
	HAL_Delay(2);
	  	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, steps & (STEP_0));
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, steps & (STEP_1));
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, steps & (STEP_2));
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, steps & (STEP_3));
}










static void single_step(motor_step steps) {

HAL_Delay(2);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, steps & (STEP_0));
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, steps & (STEP_1));
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, steps & (STEP_2));
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, steps & (STEP_3));
}




static void full_step() {
  single_step(STEP_0);
  single_step(STEP_1);
  single_step(STEP_2);
  single_step(STEP_3);
}

const int steps_per_full_rotation = 512;

const int rotate_deg = 90;

int main(void) {

	HAL_Init();

	SystemClock_Config();

	MX_GPIO_Init();

	for (int i = 0; i < steps_per_full_rotation / (360 / rotate_deg); i++) {
		full_step(1);
		HAL_Delay(2);
	}




	while (1) {
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
