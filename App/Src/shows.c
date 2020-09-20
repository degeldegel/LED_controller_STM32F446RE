/**
  ******************************************************************************
  * @file           : shows.c
  * @brief          : Implementation file of shows system
  ******************************************************************************
  *
  ******************************************************************************
  */
#include <string.h>
#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "shows.h"
#include "porting_layer.h"
#include "main.h"

/* =========================================================================================== */
/*   PRIVATE DEFINES                                                                           */
/* =========================================================================================== */
#define SHOWS_TASK_STACK_SIZE       (128*4)
#define SHOWS_TASK_PRIO             (configMAX_PRIORITIES - 2)
#define Q_SHOWS_LENGTH              (1)
#define Q_SHOWS_ITEM_SIZE           (sizeof(show_q_item_t))

/* =========================================================================================== */
/*   GLOBAL VARIABLES                                                                          */
/* =========================================================================================== */
show_db_t gp_shows[NUM_OF_SHOW] = {0};
init_show gp_init_show_func[NUM_OF_SHOW] = LIST_OF_INIT_FUNCTIONS;

TaskHandle_t  gp_shows_task_handler = NULL;
QueueHandle_t gp_show_q = NULL;

/* user interface db, the user fills up this array according to LED strip id and led number,
 * the driver will push the LED strips according to this db */
uint8_t  gp_frame[MAX_SUPPORTED_NUM_OF_STRIPS][MAX_SUPPORTED_LEDS_IN_STRIP][MAX_RGB_PER_LED] = {0};

/* Power correction multiplier, updated every frame. used in perform_power_correction function
 * by LED strip update. holds the percentage of max_power out of 1024 e.g. 20% will be 204 */
static uint32_t s_power_cor_mult = 0;
/* =========================================================================================== */
/*   STATIC VARIABLES                                                                          */
/* =========================================================================================== */

/* =========================================================================================== */
/*   PRIVATE FUNCTION                                                                          */
/* =========================================================================================== */
/**
  * @brief      prepares the frame for the set frame function of the show
  * @param      uint16_t    show id
  * @retval     void
  * @details    prepares the frame for the user show set frame function according to show configuration
  */
void prepare_frame(uint16_t show_id)
{
    switch (gp_shows[show_id].next_frame)
    {
        case NEXT_FRAME_NO_CHANGE:
        {
            /* Do nothing - No change is needed */
        } break;
        case NEXT_FRAME_MOVE_FORWARD:
        {
            int32_t led_id, strip_id;
            for (strip_id=0; strip_id < MAX_ACTIVE_STRIPS; strip_id++)
            {
                for (led_id=(MAX_LEDS_IN_STRIP-1); led_id!=0; led_id--)
                {
                    gp_frame[strip_id][led_id][RGB_GREEN] = gp_frame[strip_id][led_id-1][RGB_GREEN];
                    gp_frame[strip_id][led_id][RGB_RED]   = gp_frame[strip_id][led_id-1][RGB_RED];
                    gp_frame[strip_id][led_id][RGB_BLUE]  = gp_frame[strip_id][led_id-1][RGB_BLUE];
                }
            }
        } break;
        case NEXT_FRAME_MOVE_BACKWARD:
        {
            int32_t led_id, strip_id;
            for (strip_id=0; strip_id < MAX_ACTIVE_STRIPS; strip_id++)
            {
                for (led_id=0; led_id<(MAX_LEDS_IN_STRIP-1); led_id++)
                {
                    gp_frame[strip_id][led_id][RGB_GREEN] = gp_frame[strip_id][led_id-1][RGB_GREEN];
                    gp_frame[strip_id][led_id][RGB_RED]   = gp_frame[strip_id][led_id-1][RGB_RED];
                    gp_frame[strip_id][led_id][RGB_BLUE]  = gp_frame[strip_id][led_id-1][RGB_BLUE];
                }
            }
        } break;
        case NEXT_FRAME_RESET:
        {
            memset(gp_frame, 0, sizeof(gp_frame));
        } break;
        default:
        {
            PL_ASSERT();
        } break;
    }
}

/**
  * @brief      prepares power correction multiplier
  * @param      uint16_t    show id
  * @retval     void
  * @details    calculates power correction multiplier according to max_power configuration
  *             The correction multiplier is saved for performing the correction during mask
  *             update in LED driver.
  *             max_power is a value (0-100) in percent, in order to make the calculation more
  *             efficient we calculate the multiplier to be the percentage out of 1024.
  *             This makes the power correction calculation simply multiply by the multiplier and
  *             shift right 10 bits
  */
void prepare_power_correction(uint16_t show_id)
{
    s_power_cor_mult = 1024 * (uint32_t)gp_shows[show_id].max_power / 100;
}

/**
  * @brief      performs fade in or fade out according to configuration
  * @param      uint16_t    show id
  * @retval     void
  */
void perform_fade_in_out(uint16_t show_id)
{
    //TODO
}

/**
  * @brief      init shows function
  * @param      void
  * @retval     void
  * @details    initialize shows database and runs all initialization function of all shows
  */
void init_shows(void)
{
    uint32_t show_idx;
    for (show_idx=0; show_idx<NUM_OF_SHOW; show_idx++)
    {
        if (gp_init_show_func[show_idx])
        {
            gp_init_show_func[show_idx](show_idx, &gp_shows[show_idx]);
        }
    }
    update_led_strips(gp_frame);
    pl_start_frame_timer();
}

/**
  * @brief      Main shows task
  * @param      void
  * @param      void* additional arguments passed to task (NULL for this funciton)
  * @retval     void
  * @details    shows task implementation
  */
void shows_task(void* p_argument)
{
    show_q_item_t q_item;
    uint16_t show_id;
    init_shows();
    while (true)
    {
        PL_ASSERT_COND(xQueueReceive(gp_show_q, &q_item, portMAX_DELAY));
        show_id = q_item.show_id;
        if (gp_shows[show_id].show_mode == SHOW_MODE_FRAME_BY_FRAME)
        {
            prepare_frame(show_id);
            gp_shows[show_id].set_frame(show_id, q_item.frame_index, gp_frame);
            prepare_power_correction(show_id);
            perform_fade_in_out(show_id);
            update_led_strips(gp_frame);
        }
        else
        {
            //TODO - develop running image
            PL_ASSERT();
        }
    }
}

/* =========================================================================================== */
/*   PUBLIC FUNCTION                                                                           */
/* =========================================================================================== */
/**
  * @brief      performs power correction according to max_power configuration
  * @param      uint16_t    show id
  * @param      uint8_t*    p_color
  * @retval     void
  */
void perform_power_correction(uint8_t* p_color)
{
    uint32_t color = (uint32_t)*p_color;
    color = (color * s_power_cor_mult) >> 10;
    *p_color = (uint8_t)color;
}

/**
  * @brief      sends queue item to show task
  * @param      uint16_t    show id that should run
  * @param      uint32_t    frame index of next frame to run
  * @retval     void
  * @details    This funciton receives show id that should run the next frame and frame index
  *             that should run. starts the show task by posting to tasks queue
  */
void run_show(uint16_t show_id, uint32_t frame_idx)
{
    show_q_item_t q_item;
    q_item.show_id = show_id;
    q_item.frame_index = frame_idx;
    PL_ASSERT_COND(xQueueSend(gp_show_q, &q_item, 0));
}
/**
  * @brief      init freeRTOS resources related to the shows
  * @param      void
  * @retval     void
  * @details    initializes all resources related to shows
  */
void init_shows_rtos_resources(void)
{
    /* shows Queue */
    gp_show_q = xQueueCreate(Q_SHOWS_LENGTH, Q_SHOWS_ITEM_SIZE);

    /* check Queue creation success */
    PL_ASSERT_COND(gp_show_q);

    /* adding Queue for debug */
    vQueueAddToRegistry(gp_show_q, "Shows Q");
}

/**
  * @brief      init freeRTOS tasks related to the shows
  * @param      void
  * @retval     void
  * @details    initializes all tasks related to shows
  */
void init_shows_tasks(void)
{
    /* Init shows task */
    BaseType_t res;
    res = xTaskCreate(shows_task,                   // The task implementation
                      "shows main task",            // The task text name
                      SHOWS_TASK_STACK_SIZE,        // The task stack size
                      NULL,                         // Optional Parameter
                      SHOWS_TASK_PRIO,              // The task priority
                      &gp_shows_task_handler);      // The task handle
    PL_ASSERT_COND(pdPASS == res);
}

/***********************************TEMP FOR COMPILATION**************************************/
void stars_show_init(uint16_t show_id, show_db_t* show_db)
{

}

