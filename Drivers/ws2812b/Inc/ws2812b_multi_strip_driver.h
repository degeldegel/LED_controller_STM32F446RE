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
void drive_LED_strips(void);

/**
  * @brief  Function updates driver's strips database based on frame received from the user
  * @param  void
  * @retval void
  */
void update_LED_strips(uint8_t*** frame);

/**
  * @brief  Performs initialization of all strips and masks used by the multi strip driver.
  *         Also drives cleared output to shut all active LEDs in strips.
  * @param  void
  * @retval void
  */
void init_LED_strips(void);

#endif  /* _WS2812B_MULTI_STRIP_DRIVER_H */

