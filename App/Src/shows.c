/**
  ******************************************************************************
  * @file           : shows.c
  * @brief          : Implementation file of shows system
  ******************************************************************************
  *
  ******************************************************************************
  */
#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "shows.h"
#include "porting_layer.h"

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
uint8_t  gp_frame[MAX_SUPPORTED_NUM_OF_STRIPS][MAX_SUPPORTED_LEDS_IN_STRIP][NUM_OF_CFG_BYTES_PER_LED] = {0};

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
        } break;
        case NEXT_FRAME_MOVE_FORWARD:
        {
        } break;
        case NEXT_FRAME_MOVE_BACKWARD:
        {
        } break;
        case NEXT_FRAME_RESET:
        {
        } break;
        default:
        {
            PL_ASSERT();
        } break;
    }
}

/**
  * @brief      performs power correction according to max_power configuration
  * @param      uint16_t    show id
  * @retval     void
  */
void perform_power_correction(uint16_t show_id)
{
    //TODO
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

/* =========================================================================================== */
/*   PUBLIC FUNCTION                                                                           */
/* =========================================================================================== */
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
            gp_shows[show_id].set_frame(show_id, gp_frame);
            perform_power_correction(show_id);
            perform_fade_in_out(show_id);
        }
        else
        {
            //TODO - develop running image
            PL_ASSERT();
        }
    }
}

/**
  * @brief      init freeRTOS resources related to the shows
  * @param      void
  * @retval     void
  * @details    initializes all resources related to shows
  */
void shows_initRtosResources(void)
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
void shows_initTasks(void)
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
