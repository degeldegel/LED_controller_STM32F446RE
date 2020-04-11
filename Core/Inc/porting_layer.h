/*
 * porting_layer.h
 *
 *  Created on: Apr 11, 2020
 *      Author: danielg
 */

#ifndef INC_PORTING_LAYER_H_
#define INC_PORTING_LAYER_H_
#include <FreeRTOS.h>
#include <task.h>
/* =========================================================================================== */
/*   DEFINES                                                                                    */
/* =========================================================================================== */
#define true            (1)
#define false           (0)

/* =========================================================================================== */
/*   MACROS                                                                                    */
/* =========================================================================================== */
#define PL_ASSERT_COND(cond)        {if(!(cond)) pl_assert_handle();}
#define PL_ASSERT()                 {pl_assert_handle();}

/* =========================================================================================== */
/*   PUBLIC FUNCTION DECLARATION                                                               */
/* =========================================================================================== */
/**
  * @brief  This function performs assert handling
  * @param  void
  * @retval void
  */
void pl_assert_handle(void);

#endif /* INC_PORTING_LAYER_H_ */
