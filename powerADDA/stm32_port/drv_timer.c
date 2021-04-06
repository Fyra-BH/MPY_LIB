#include "stm32f4xx_hal.h"
#include "stdio.h"

TIM_HandleTypeDef htim4;

void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 63;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 1000 - 1;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  // htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    // Error_Handler();
    printf("Error_Handler();\n");
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    // Error_Handler();
    printf("Error_Handler();\n");
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    // Error_Handler();
    printf("Error_Handler();\n");
  }

  //HAL_TIM_Base_Start_IT(&htim4);
  printf("OK\n");
}
void start_tim4()
{
  __HAL_RCC_TIM4_CLK_ENABLE();
  /* TIM4 interrupt Init */
  HAL_NVIC_SetPriority(TIM4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM4_IRQn);
  HAL_TIM_Base_Start_IT(&htim4);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *TIM)
{
  static int cnt = 0;
  if (TIM->Instance == TIM4)
  {
    cnt++;
    if (cnt == 100)
    {
      cnt = 0;
      printf("GG\n");
    }
  }
}

// void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
// {
//   if(htim_base->Instance==TIM4)
//   {
//   /* USER CODE BEGIN TIM4_MspInit 0 */

//   /* USER CODE END TIM4_MspInit 0 */
//     /* Peripheral clock enable */
//     __HAL_RCC_TIM4_CLK_ENABLE();
//     /* TIM4 interrupt Init */
//     HAL_NVIC_SetPriority(TIM4_IRQn, 10, 0);
//     HAL_NVIC_EnableIRQ(TIM4_IRQn);
//   /* USER CODE BEGIN TIM4_MspInit 1 */

//   /* USER CODE END TIM4_MspInit 1 */
//   }
//   else if(htim_base->Instance==TIM5)
//   {
//   /* USER CODE BEGIN TIM5_MspInit 0 */

//   /* USER CODE END TIM5_MspInit 0 */
//     /* Peripheral clock enable */
//     __HAL_RCC_TIM5_CLK_ENABLE();
//     /* TIM5 interrupt Init */
//     HAL_NVIC_SetPriority(TIM5_IRQn, 0, 0);
//     HAL_NVIC_EnableIRQ(TIM5_IRQn);
//   /* USER CODE BEGIN TIM5_MspInit 1 */

//   /* USER CODE END TIM5_MspInit 1 */
//   }

// }
