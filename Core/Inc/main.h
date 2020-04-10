/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define LED_STRIP_1_Pin GPIO_PIN_0
#define LED_STRIP_1_GPIO_Port GPIOC
#define LED_STRIP_3_Pin GPIO_PIN_1
#define LED_STRIP_3_GPIO_Port GPIOC
#define LED_STRIP_5_Pin GPIO_PIN_2
#define LED_STRIP_5_GPIO_Port GPIOC
#define LED_STRIP_7_Pin GPIO_PIN_3
#define LED_STRIP_7_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define LED_STRIP_9_Pin GPIO_PIN_4
#define LED_STRIP_9_GPIO_Port GPIOC
#define LED_STRIP_11_Pin GPIO_PIN_5
#define LED_STRIP_11_GPIO_Port GPIOC
#define LED_STRIP_0_Pin GPIO_PIN_0
#define LED_STRIP_0_GPIO_Port GPIOB
#define LED_STRIP_2_Pin GPIO_PIN_1
#define LED_STRIP_2_GPIO_Port GPIOB
#define LED_STRIP_4_Pin GPIO_PIN_2
#define LED_STRIP_4_GPIO_Port GPIOB
#define LED_STRIP_13_Pin GPIO_PIN_6
#define LED_STRIP_13_GPIO_Port GPIOC
#define LED_STRIP_15_Pin GPIO_PIN_7
#define LED_STRIP_15_GPIO_Port GPIOC
#define LED_STRIP_17_Pin GPIO_PIN_8
#define LED_STRIP_17_GPIO_Port GPIOC
#define LED_STRIP_19_Pin GPIO_PIN_9
#define LED_STRIP_19_GPIO_Port GPIOC
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define LED_STRIP_6_Pin GPIO_PIN_3
#define LED_STRIP_6_GPIO_Port GPIOB
#define LED_STRIP_8_Pin GPIO_PIN_4
#define LED_STRIP_8_GPIO_Port GPIOB
#define LED_STRIP_10_Pin GPIO_PIN_5
#define LED_STRIP_10_GPIO_Port GPIOB
#define LED_STRIP_12_Pin GPIO_PIN_6
#define LED_STRIP_12_GPIO_Port GPIOB
#define LED_STRIP_14_Pin GPIO_PIN_7
#define LED_STRIP_14_GPIO_Port GPIOB
#define LED_STRIP_16_Pin GPIO_PIN_8
#define LED_STRIP_16_GPIO_Port GPIOB
#define LED_STRIP_18_Pin GPIO_PIN_9
#define LED_STRIP_18_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
