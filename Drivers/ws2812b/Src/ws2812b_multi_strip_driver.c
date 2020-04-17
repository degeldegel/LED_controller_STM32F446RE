#include "stm32f4xx_hal.h"
#include "ws2812b_multi_strip_driver.h"
#include "main.h"

/* =========================================================================================== */
/*   PRIVATE DEFINES                                                                           */
/* =========================================================================================== */
#define MAX_SUPPORTED_NUM_OF_PORTS (2)
#define BITS_TO_CONFIGURE_ONE_LED (24)
#define BITS_IN_BYTE (8)
#define GPIO_PRT_NA (255)
#define GPIO_PIN_NA ((uint16_t)0x0000)
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
/* Private MACROS                                                                              */
/* =========================================================================================== */
#define GET_STRIP_PORT(strip) (strip_ports[strip])
#define GET_STRIP_GPIO(strip) (strip_GPIOs[strip])

/* =========================================================================================== */
/* STATIC VARIABLES                                                                            */
/* =========================================================================================== */
/*contains masks of all bits that are zero in PortB/C*/
uint16_t GPIO_strips_mask[MAX_SUPPORTED_NUM_OF_PORTS][MAX_SUPPORTED_LEDS_IN_STRIP * BITS_TO_CONFIGURE_ONE_LED];

/*active GPIOs in the port 0 - portB, 1 - portC */
uint16_t GPIO_all_strips_mask[MAX_SUPPORTED_NUM_OF_PORTS];

uint8_t strip_ports[MAX_SUPPORTED_NUM_OF_STRIPS] =
         /*  Strip #0               Strip #1                Strip #2                Strip #3                Strip #4  */
        {LED_STRIP_0_Port,      LED_STRIP_1_Port,       LED_STRIP_2_Port,       LED_STRIP_3_Port,       LED_STRIP_4_Port,
         /*  Strip #5               Strip #6                Strip #7                Strip #8                Strip #9  */
         LED_STRIP_5_Port,      LED_STRIP_6_Port,       LED_STRIP_7_Port,       LED_STRIP_8_Port,       LED_STRIP_9_Port,
         /*  Strip #10              Strip #11               Strip #12               Strip #13               Strip #14 */
         LED_STRIP_10_Port,     LED_STRIP_11_Port,      LED_STRIP_12_Port,      LED_STRIP_13_Port,      LED_STRIP_14_Port,
         /*  Strip #15              Strip #16               Strip #17               Strip #18               Strip #19 */
         LED_STRIP_15_Port,     LED_STRIP_16_Port,      LED_STRIP_17_Port,      LED_STRIP_18_Port,      LED_STRIP_19_Port};
uint16_t strip_GPIOs[MAX_SUPPORTED_NUM_OF_STRIPS] =
         /*  Strip #0               Strip #1                Strip #2                Strip #3                Strip #4  */
        {LED_STRIP_0_Pin,       LED_STRIP_1_Pin,        LED_STRIP_2_Pin,        LED_STRIP_3_Pin,        LED_STRIP_4_Pin,
         /*  Strip #5               Strip #6                Strip #7                Strip #8                Strip #9  */
         LED_STRIP_5_Pin,       LED_STRIP_6_Pin,        LED_STRIP_7_Pin,        LED_STRIP_8_Pin,        LED_STRIP_9_Pin,
         /*  Strip #10              Strip #11               Strip #12               Strip #13               Strip #14 */
         LED_STRIP_10_Pin,      LED_STRIP_11_Pin,       LED_STRIP_12_Pin,       LED_STRIP_13_Pin,       LED_STRIP_14_Pin,
         /*  Strip #15              Strip #16               Strip #17               Strip #18               Strip #19 */
         LED_STRIP_15_Pin,      LED_STRIP_16_Pin,       LED_STRIP_17_Pin,       LED_STRIP_18_Pin,       LED_STRIP_19_Pin};

/* =========================================================================================== */
/* PRIVATE FUNCTIONS                                                                           */
/* =========================================================================================== */
/**
  * @brief  This functions performs the actual update of the GPIOs of the strip LEDs. it is using the masks updated in function
  *         update_driver_masks. this is the heart of the multi strip driver, it updates the PORTs of the GPIOs, this enables it
  *         to update multiple GPIOs on parallel. The update timing is according to ws2812b strip LED specifications.
  * @param  void
  * @retval void
  */
void drive_ws2812b_led_strips_via_gpio_ports(void)
{
	int curr_led_bit_idx;
	uint16_t curr_zero_mask[2];
	uint32_t cycle_time_psec, cycles_of_350_nano, cycles_of_900_nano, cycles_of_1250_nano;
    cycle_time_psec = 1000000/(SystemCoreClock/1000000); //cycletime in picksec
    cycles_of_350_nano = 350000/cycle_time_psec - CYCLES_DELAY_COMPENSATION;
    cycles_of_900_nano = 900000/cycle_time_psec - CYCLES_DELAY_COMPENSATION;
    cycles_of_1250_nano = 1250000/cycle_time_psec - CYCLES_DELAY_COMPENSATION;
	SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
	for (curr_led_bit_idx=0; curr_led_bit_idx < MAX_LEDS_IN_STRIP * BITS_TO_CONFIGURE_ONE_LED; curr_led_bit_idx++)
	{
	    DWT->CYCCNT = 0;
		//raise all Strips up
		GPIOB->ODR |=  GPIO_all_strips_mask[GPIO_PORT_B];
		GPIOC->ODR |=  GPIO_all_strips_mask[GPIO_PORT_C];
		//wait for strips with bit value zero
		while(DWT->CYCCNT < cycles_of_350_nano) {};
		//lower all strips with bit value zero
		curr_zero_mask[GPIO_PORT_B] = GPIO_strips_mask[GPIO_PORT_B][curr_led_bit_idx];
		curr_zero_mask[GPIO_PORT_C] = GPIO_strips_mask[GPIO_PORT_C][curr_led_bit_idx];
		GPIOB->ODR &= ~curr_zero_mask[GPIO_PORT_B];
		GPIOC->ODR &= ~curr_zero_mask[GPIO_PORT_C];
		//wait for strips with bit value one
		while(DWT->CYCCNT < cycles_of_900_nano) {};
		//lower all strips with bit value zero
		GPIOB->ODR &= ~(curr_zero_mask[GPIO_PORT_B] ^ GPIO_all_strips_mask[GPIO_PORT_B]);
		GPIOC->ODR &= ~(curr_zero_mask[GPIO_PORT_C] ^ GPIO_all_strips_mask[GPIO_PORT_C]);
		//wait for end of bit configuration cycle ~1.25 msec
		while(DWT->CYCCNT < cycles_of_1250_nano) {};
	}
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
}

/**
  * @brief  inner function used to clear old GPIO strips masks, before the update takes effect.
  * @param  void
  * @retval void
  */
void zero_all_driver_masks(void)
{
	int curr_led_bit_idx, curr_GPIO_port_id;
	for (curr_GPIO_port_id=0; curr_GPIO_port_id<MAX_SUPPORTED_NUM_OF_PORTS; curr_GPIO_port_id++)
	{
        for (curr_led_bit_idx=0; curr_led_bit_idx < MAX_LEDS_IN_STRIP * BITS_TO_CONFIGURE_ONE_LED; curr_led_bit_idx++)
        {
            GPIO_strips_mask[curr_GPIO_port_id][curr_led_bit_idx] = 0;
        }
	}
}

/**
  * @brief  This function performs the conversion between the user interface AKA LED_strips and the driver db
  * @param  uint8_t[strip_id][led_id][color] - pointer to led strips database
  * @retval void
  */
void update_driver_masks(uint8_t LED_strips[MAX_SUPPORTED_NUM_OF_STRIPS][MAX_SUPPORTED_LEDS_IN_STRIP][NUM_OF_CFG_BYTES_PER_LED])
{
	int led_idx, rgb_idx, strip_idx;
	zero_all_driver_masks();
	for (strip_idx=0; strip_idx < MAX_ACTIVE_STRIPS; strip_idx++)
	{
		int curr_strip_port, curr_strip_gpio_msk;
		curr_strip_port = GET_STRIP_PORT(strip_idx);
		curr_strip_gpio_msk = GET_STRIP_GPIO(strip_idx);
		for (led_idx=0; led_idx < MAX_LEDS_IN_STRIP; led_idx++)
		{
			for (rgb_idx=0; rgb_idx < NUM_OF_CFG_BYTES_PER_LED; rgb_idx++)
			{
				uint8_t cur_rgb_led;
				cur_rgb_led = LED_strips[strip_idx][led_idx][rgb_idx];
				int base_bit_idx = led_idx*BITS_TO_CONFIGURE_ONE_LED + rgb_idx*BITS_IN_BYTE;
				GPIO_strips_mask[curr_strip_port][base_bit_idx + 0] |= ((cur_rgb_led>>7) & 0x1) ? 0 : curr_strip_gpio_msk;
				GPIO_strips_mask[curr_strip_port][base_bit_idx + 1] |= ((cur_rgb_led>>6) & 0x1) ? 0 : curr_strip_gpio_msk;
				GPIO_strips_mask[curr_strip_port][base_bit_idx + 2] |= ((cur_rgb_led>>5) & 0x1) ? 0 : curr_strip_gpio_msk;
				GPIO_strips_mask[curr_strip_port][base_bit_idx + 3] |= ((cur_rgb_led>>4) & 0x1) ? 0 : curr_strip_gpio_msk;
				GPIO_strips_mask[curr_strip_port][base_bit_idx + 4] |= ((cur_rgb_led>>3) & 0x1) ? 0 : curr_strip_gpio_msk;
				GPIO_strips_mask[curr_strip_port][base_bit_idx + 5] |= ((cur_rgb_led>>2) & 0x1) ? 0 : curr_strip_gpio_msk;
				GPIO_strips_mask[curr_strip_port][base_bit_idx + 6] |= ((cur_rgb_led>>1) & 0x1) ? 0 : curr_strip_gpio_msk;
				GPIO_strips_mask[curr_strip_port][base_bit_idx + 7] |= ((cur_rgb_led>>0) & 0x1) ? 0 : curr_strip_gpio_msk;
			}
		}
	}
}

/**
  * @brief  inner function used to update the mask of the active GPIOs of the strips
  * @param  which GPIO port should be updated
  * @param  mask of all active strips belong to the GPIO port
  *
  * @retval void
  */
void update_gpio_all_strips_mask(uint8_t port, uint16_t update_mask)
{
    GPIO_all_strips_mask[port] = update_mask;
}

/* =========================================================================================== */
/* PUBLIC FUNCTIONS                                                                            */
/* =========================================================================================== */
/**
  * @brief  main function that drives the strips
  * @param  void
  * @retval void
  */
void drive_led_strips(void)
{
    drive_ws2812b_led_strips_via_gpio_ports();
}

/**
  * @brief  Function updates driver's strips database based on frame received from the user
  * @param  uint8_t[strip_id][led_id][color] - pointer to frame
  * @retval void
  */
void update_led_strips(uint8_t frame[MAX_SUPPORTED_NUM_OF_STRIPS][MAX_SUPPORTED_LEDS_IN_STRIP][NUM_OF_CFG_BYTES_PER_LED])
{
    update_driver_masks(frame);
}

/**
  * @brief  Performs initialization of all strips and masks used by the multi strip driver.
  *         Also drives cleared output to shut all active LEDs in strips.
  * @param  void
  * @retval void
  */
void init_led_strips(void)
{
    uint8_t led_strip_idx;
    uint16_t port_b_active_strips_mask = GPIO_PIN_NA, port_c_active_strips_mask = GPIO_PIN_NA;
    for (led_strip_idx = 0; led_strip_idx<MAX_SUPPORTED_NUM_OF_STRIPS; led_strip_idx++)
    {
        if (strip_ports[led_strip_idx] == GPIO_PORT_B)
        {
            port_b_active_strips_mask |= strip_GPIOs[led_strip_idx];
        }
        else if (strip_ports[led_strip_idx] == GPIO_PORT_C)
        {
            port_c_active_strips_mask |= strip_GPIOs[led_strip_idx];
        }
    }
    update_gpio_all_strips_mask(GPIO_PORT_B, port_b_active_strips_mask);
    update_gpio_all_strips_mask(GPIO_PORT_C, port_c_active_strips_mask);

    // enable DWT counter used for timing of the driver
    DWT->CTRL |= 1;

    drive_led_strips();
}
