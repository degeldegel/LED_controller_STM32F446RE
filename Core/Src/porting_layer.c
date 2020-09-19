/*
 * porting_layer.c
 *
 *  Created on: Apr 11, 2020
 *      Author: danielg
 */

#include "porting_layer.h"
#include "main.h"
#include "tim.h"

/* =========================================================================================== */
/*   GLOBAL VARIABLES                                                                          */
/* =========================================================================================== */

/* =========================================================================================== */
/*   STATIC VARIABLES                                                                          */
/* =========================================================================================== */

/* =========================================================================================== */
/*   PRIVATE FUNCTION                                                                          */
/* =========================================================================================== */

/* =========================================================================================== */
/*   PUBLIC FUNCTION                                                                           */
/* =========================================================================================== */
/**
  * @brief  This function performs assert handling
  * @param  void
  * @retval void
  */
void pl_start_frame_timer(void)
{
    HAL_TIM_Base_Start_IT(&htim3);
}

/**
  * @brief  This function performs assert handling
  * @param  void
  * @retval void
  */
void pl_assert_handle(void)
{
    volatile uint32_t cntr;
    taskENTER_CRITICAL();
    while (true)
    {
        HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
        cntr=10000000;
        while(cntr--){}
    }
}
