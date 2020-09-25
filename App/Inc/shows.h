/**
  ******************************************************************************
  * @file           : shows.h
  * @brief          : header file of shows system
  ******************************************************************************
  *
  ******************************************************************************
  */

#ifndef _SHOWS_H
#define _SHOWS_H

#include "ws2812b_multi_strip_driver.h"

/* =========================================================================================== */
/*   DEFINES AND ENUMERATORS                                                                   */
/* =========================================================================================== */
typedef enum _show_mode
{
    SHOW_MODE_FRAME_BY_FRAME,
    SHOW_MODE_RUNNING_IMAGE,
} show_mode_e;

typedef enum _next_frame
{
    NEXT_FRAME_NO_CHANGE,
    NEXT_FRAME_MOVE_FORWARD,
    NEXT_FRAME_MOVE_BACKWARD,
    NEXT_FRAME_RESET,
} next_frame_e;

/* =========================================================================================== */
/*   STRUCTS AND TYPE DEFINITIONS                                                                               */
/* =========================================================================================== */
/**
  * @brief   Set frame function receives pointer to frame, and updates it for next frame
  * @param   uint16_t - show id
  * @param   uint32_t - Running frame index, index 0 is first frame of show
  * @param   uint8_t[strip_id][led_id][color] - pointer to frame
  * @retval  void
  * @details This function will receive a pointer to the frame, which will be already prepared according to the
  *          next_frame configuration of the show database. It will update the frame with the new frame that
  *          will be shown
  */
typedef void (*set_frame_func)(uint16_t show_id, uint32_t frame_idx, uint8_t frame[MAX_SUPPORTED_NUM_OF_STRIPS][MAX_SUPPORTED_LEDS_IN_STRIP][MAX_RGB_PER_LED]);

typedef struct show_db
{
    show_mode_e     show_mode;          /* What kind of a show it is */
    next_frame_e    next_frame;         /* how to prepare next frame for the set_frame function */
    uint16_t        frame_duration;     /* how long this frame will run, in msec, value range: 20-1000 */
    uint8_t         fade_in_enable;     /* boolean - perform fade in on start of show */
    uint8_t         fade_out_enable;    /* boolean - perform fade out on end of show */
    uint8_t         max_power;          /* maximum power of the LEDS in percentage, values 0-100 */
    uint8_t         direction;          /* direction of the show, 0 - from entry of strip to end, 1 - from end of strip to the entry */
    uint8_t         image_speed;        /* move image every image_speed frames (RUNNING_IMAGE only) */
    set_frame_func  set_frame;          /* pointer to the set frame function of the show (FRAME_BY_FRAME only) */
    uint8_t*        image;              /* pointer to the image */
} show_db_t;

typedef struct show_q_item
{
    uint16_t        show_id;            /* show_id*/
    uint32_t        frame_index;        /* frame_idx */
} show_q_item_t;

/**
  * @brief      initialization of a show
  * @param      uint16_t   - show id
  * @retval     show_db_t* - pointer to show database
  * @details    This function will initialize the show
  *             This function will return a pointer to the show's show database.
  *             This function will be called on initialization of the system.
  */
typedef show_db_t* (*init_show)(uint16_t show_id);

/* =========================================================================================== */
/*   MACROS                                                                                    */
/* =========================================================================================== */

/* =========================================================================================== */
/*   USER SHOWS DEFINITIONS AND DECLARATIONS                                                   */
/* =========================================================================================== */
typedef enum _show_id
{
    SHOW_ID_SNAKE,
    SHOW_ID_STARS,
    NUM_OF_SHOW
} show_id_e;

show_db_t* snake_show_init(uint16_t show_id);
show_db_t* stars_show_init(uint16_t show_id);

#define LIST_OF_INIT_FUNCTIONS {snake_show_init, \
		                        stars_show_init}

/* =========================================================================================== */
/*   PUBLIC FUNCTION DECLARATION                                                               */
/* =========================================================================================== */
/**
  * @brief      sends queue item to show task
  * @param      uint16_t    show id that should run
  * @param      uint32_t    frame index of next frame to run
  * @retval     void
  * @details    This funciton receives show id that should run the next frame and frame index
  *             that should run. starts the show task by posting to tasks queue
  */
void run_show(uint16_t show_id, uint32_t frame_idx);

/**
  * @brief      init freeRTOS resources related to the shows
  * @param      void
  * @retval     void
  * @details    initializes all resources related to shows
  */
void init_shows_rtos_resources(void);

/**
  * @brief      init freeRTOS tasks related to the shows
  * @param      void
  * @retval     void
  * @details    initializes all tasks related to shows
  */
void init_shows_tasks(void);

#endif  /* _SHOWS_H */

