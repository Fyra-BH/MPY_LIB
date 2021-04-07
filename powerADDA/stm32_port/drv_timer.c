/**
 * @file drv_timer.c
 * @author Fyra-BH (fyra@foxmail.com)
 * @brief 初始化TIM4，中断频率=采样率
 * @version 0.1
 * @date 2021-04-07
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "stm32f4xx_hal.h"
#include "rque_fifo.h"
#include "stdio.h"

TIM_HandleTypeDef htim4;

void POWERADDA_TIM4_Init(void)
{

  /* USER CODE BEGIN POWERADDA_TIM4_Init 0 */

  /* USER CODE END POWERADDA_TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN POWERADDA_TIM4_Init 1 */

  /* USER CODE END POWERADDA_TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 0;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 1750 - 1;
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

}

void timer4_start_it(void)
{
  HAL_TIM_Base_Start_IT(&htim4);
}


/**
* @brief TIM_Base MSP Initialization
* This function configures the hardware resources used in this example
* @param htim_base: TIM_Base handle pointer
* @retval None
*/
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
  if(htim_base->Instance==TIM4)
  {
  /* USER CODE BEGIN TIM4_MspInit 0 */

  /* USER CODE END TIM4_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_TIM4_CLK_ENABLE();
    /* TIM4 interrupt Init */
    HAL_NVIC_SetPriority(TIM4_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM4_IRQn);
  /* USER CODE BEGIN TIM4_MspInit 1 */

  /* USER CODE END TIM4_MspInit 1 */
  }
}

/**
* @brief TIM_Base MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param htim_base: TIM_Base handle pointer
* @retval None
*/
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{
  if(htim_base->Instance==TIM4)
  {
  /* USER CODE BEGIN TIM4_MspDeInit 0 */

  /* USER CODE END TIM4_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM4_CLK_DISABLE();

    /* TIM4 interrupt DeInit */
    HAL_NVIC_DisableIRQ(TIM4_IRQn);
  /* USER CODE BEGIN TIM4_MspDeInit 1 */

  /* USER CODE END TIM4_MspDeInit 1 */
  }
}

extern DAC_HandleTypeDef hdac;
extern rque_t Q_dac;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *TIM)
{
  static int cnt = 0;

  if (TIM->Instance == TIM4)
  {	
    uint8_t tmp = 0;
    uint16_t data = 0;
    if (rque_read(&Q_dac, &tmp)) return;
    else
    {
      data = tmp * 256;//先读高字节
      rque_read(&Q_dac, &tmp);
      data += tmp;//再读低字节
    }
    HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, data);	

    cnt++;
    if (cnt == 48000)
    {
      cnt = 0;
      // printf("GG\n");
    }
  }
}