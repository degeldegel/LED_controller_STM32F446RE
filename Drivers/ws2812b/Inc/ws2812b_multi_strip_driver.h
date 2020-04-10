#ifndef _WS2812B_MULTI_STRIP_DRIVER_H
#define _WS2812B_MULTI_STRIP_DRIVER_H

#include "main.h"

/* =========================================================================================== */
/* ==  DEFINES == */
/* =========================================================================================== */

#define MAX_SUPPORTED_NUM_OF_STRIPS (20)
#define MAX_SUPPORTED_LEDS_IN_STRIP (300)
#define MAX_SUPPORTED_NUM_OF_PORTS (2)
#define NUM_OF_CFG_BYTES_PER_LED (3)
#define BITS_TO_CONFIGURE_ONE_LED (24)
#define MAX_LEDS_IN_STRIP (300)
#define MAX_ACTIVE_STRIPS (4)
#define BITS_IN_BYTE (8)
#define GPIO_PRT_NA (255)
#define GPIO_PIN_NA ((uint16_t)0x0000)
#define LED_MAX_POWER (150)
#define CYCLES_DELAY_COMPENSATION (12)
#define GPIO_PORT_B (0)
#define GPIO_PORT_C (1)
#define LED_STRIP_0_Port  (LED_STRIP_0_GPIO_Port  == GPIOB ? GPIO_PORT_B : GPIO_PORT_C)
#define LED_STRIP_1_Port  (LED_STRIP_1_GPIO_Port  == GPIOB ? GPIO_PORT_B : GPIO_PORT_C)
#define LED_STRIP_2_Port  (LED_STRIP_2_GPIO_Port  == GPIOB ? GPIO_PORT_B : GPIO_PORT_C)
#define LED_STRIP_3_Port  (LED_STRIP_3_GPIO_Port  == GPIOB ? GPIO_PORT_B : GPIO_PORT_C)
#define LED_STRIP_4_Port  (LED_STRIP_4_GPIO_Port  == GPIOB ? GPIO_PORT_B : GPIO_PORT_C)
#define LED_STRIP_5_Port  (LED_STRIP_5_GPIO_Port  == GPIOB ? GPIO_PORT_B : GPIO_PORT_C)
#define LED_STRIP_6_Port  (LED_STRIP_6_GPIO_Port  == GPIOB ? GPIO_PORT_B : GPIO_PORT_C)
#define LED_STRIP_7_Port  (LED_STRIP_7_GPIO_Port  == GPIOB ? GPIO_PORT_B : GPIO_PORT_C)
#define LED_STRIP_8_Port  (LED_STRIP_8_GPIO_Port  == GPIOB ? GPIO_PORT_B : GPIO_PORT_C)
#define LED_STRIP_9_Port  (LED_STRIP_9_GPIO_Port  == GPIOB ? GPIO_PORT_B : GPIO_PORT_C)
#define LED_STRIP_10_Port (LED_STRIP_10_GPIO_Port == GPIOB ? GPIO_PORT_B : GPIO_PORT_C)
#define LED_STRIP_11_Port (LED_STRIP_11_GPIO_Port == GPIOB ? GPIO_PORT_B : GPIO_PORT_C)
#define LED_STRIP_12_Port (LED_STRIP_12_GPIO_Port == GPIOB ? GPIO_PORT_B : GPIO_PORT_C)
#define LED_STRIP_13_Port (LED_STRIP_13_GPIO_Port == GPIOB ? GPIO_PORT_B : GPIO_PORT_C)
#define LED_STRIP_14_Port (LED_STRIP_14_GPIO_Port == GPIOB ? GPIO_PORT_B : GPIO_PORT_C)
#define LED_STRIP_15_Port (LED_STRIP_15_GPIO_Port == GPIOB ? GPIO_PORT_B : GPIO_PORT_C)
#define LED_STRIP_16_Port (LED_STRIP_16_GPIO_Port == GPIOB ? GPIO_PORT_B : GPIO_PORT_C)
#define LED_STRIP_17_Port (LED_STRIP_17_GPIO_Port == GPIOB ? GPIO_PORT_B : GPIO_PORT_C)
#define LED_STRIP_18_Port (LED_STRIP_18_GPIO_Port == GPIOB ? GPIO_PORT_B : GPIO_PORT_C)
#define LED_STRIP_19_Port (LED_STRIP_19_GPIO_Port == GPIOB ? GPIO_PORT_B : GPIO_PORT_C)

/* =========================================================================================== */
/* ==  ENUMS == */
/* =========================================================================================== */

enum {
	GREEN 	= 0,
	RED 	= 1,
	BLUE 	= 2
};

/* =========================================================================================== */
/* ==  MACROS == */
/* =========================================================================================== */

#define GET_STRIP_PORT(strip) (strip_ports[strip])
#define GET_STRIP_GPIO(strip) (strip_GPIOs[strip])

/* =========================================================================================== */
/* ==  EXTERNAL FUNCTIONS== */
/* =========================================================================================== */

/**
  * @brief  main function that drives the strips, it should be called after the LED_strips db is updated with the new output.
  *         all strips are updated on parallel.
  * @param  void
  * @retval void
  */
void drive_LED_strips(void);

/**
  * @brief  Performs initialization of all strips and masks used by the multi strip driver.
  *         Also drives cleared output to shut all active LEDs in strips.
  * @param  void
  * @retval void
  */
void init_LED_strips(void);

#endif  /* _WS2812B_MULTI_STRIP_DRIVER_H */

