#ifndef _WS2812B_MULTI_STRIP_DRIVER_H
#define _WS2812B_MULTI_STRIP_DRIVER_H

/* =========================================================================================== */
/*   DEFINES                                                                                   */
/* =========================================================================================== */
#define MAX_SUPPORTED_NUM_OF_STRIPS (20)
#define MAX_SUPPORTED_LEDS_IN_STRIP (300)
#define NUM_OF_CFG_BYTES_PER_LED (3)

/* =========================================================================================== */
/*   ENUMERATORS                                                                               */
/* =========================================================================================== */

enum {
	GREEN 	= 0,
	RED 	= 1,
	BLUE 	= 2
};

/* =========================================================================================== */
/*   PUBLIC FUNCTIONS                                                                        */
/* =========================================================================================== */
/**
  * @brief  main function that drives the strips
  * @param  void
  * @retval void
  */
void drive_led_strips(void);

/**
  * @brief  Function updates driver's strips database based on frame received from the user
  * @param  uint8_t[strip_id][led_id][color] - pointer to frame
  * @retval void
  */
void update_led_strips(uint8_t frame[MAX_SUPPORTED_NUM_OF_STRIPS][MAX_SUPPORTED_LEDS_IN_STRIP][NUM_OF_CFG_BYTES_PER_LED]);

/**
  * @brief  Releases driver's main task semaphore to run driver
  * @param  void
  * @retval void
  * @attention This function should run from interrupt only
  */
void start_led_driver_run(void);

/**
  * @brief  Performs initialization of all strips and masks used by the multi strip driver.
  *         Also drives cleared output to shut all active LEDs in strips.
  * @param  void
  * @retval void
  */
void init_led_strips(void);

/**
  * @brief      init freeRTOS resources related to the led driver
  * @param      void
  * @retval     void
  * @details    initializes all resources related to led driver
  */
void init_led_driver_rtos_resources(void);

/**
  * @brief      init freeRTOS tasks related to the led driver
  * @param      void
  * @retval     void
  * @details    initializes all tasks related to led driver
  */
void init_led_driver_tasks(void);

#endif  /* _WS2812B_MULTI_STRIP_DRIVER_H */

