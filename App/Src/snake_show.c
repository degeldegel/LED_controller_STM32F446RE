/**
  ******************************************************************************
  * @file           : snake_show.c
  * @brief          : Implementation file of snake show
  ******************************************************************************
  *
  ******************************************************************************
  */
#include <string.h>
#include "stm32f4xx_hal.h"
#include "shows.h"
#include "porting_layer.h"
#include "snake_show.h"
#include "stdlib.h"
//#include "main.h"

/* =========================================================================================== */
/*   PRIVATE DEFINES                                                                           */
/* =========================================================================================== */

/* =========================================================================================== */
/*   STATIC VARIABLES                                                                          */
/* =========================================================================================== */
static snake_show_db_t s_snake_db;

/* =========================================================================================== */
/*   PRIVATE FUNCTION                                                                          */
/* =========================================================================================== */
void snake_db_init_defaults(void)
{
    s_snake_db.config.cycle_length = 10;
    s_snake_db.config.snake_length = 7;
    s_snake_db.config.step_size = 2;

    s_snake_db.params.step_cntr = 0;
    s_snake_db.params.cycle_cntr = 0;
    for (uint32_t rgb_idx; rgb_idx < MAX_RGB_PER_LED; rgb_idx++)
    {
        s_snake_db.params.color[rgb_idx] = 0;
    }
}

void snake_show_set_frame(uint16_t show_id, uint32_t frame_idx, uint8_t frame[MAX_SUPPORTED_NUM_OF_STRIPS][MAX_SUPPORTED_LEDS_IN_STRIP][MAX_RGB_PER_LED])
{
    /* step occurs */
    if (s_snake_db.params.step_cntr == 0)
    {
        /* cycle starts - choose new random color for every new snake */
        if (s_snake_db.params.cycle_cntr == 0)
        {
            uint8_t rgb_idx;
            for (rgb_idx=0; rgb_idx<MAX_RGB_PER_LED; rgb_idx++)
            {
                s_snake_db.params.color[rgb_idx] = rand() & 0xff;
//                if (snake_show_id == 2)
//                {
//                    percent_of_rgb[rgb_idx] = rainbow[color_index][rgb_idx];
//                }
            }
//            color_index++;
//            if (color_index >= 12)
//            {
//                color_index = 0;
//            }
        }
        /* update the first led, the only one that wasn't updated till now */
//        new_led_idx = ((shows[snake_show_id].direction == REGULAR_DIRECTION) || ((shows[snake_show_id].direction == ALTER_DIRECTION) && (strip_id % 2 == 0))) ? 0 : MAX_LEDS_IN_STRIP-1;
        uint8_t shift = ((s_snake_db.params.cycle_cntr == 0) || (s_snake_db.params.cycle_cntr == (s_snake_db.config.snake_length-1))) ? 5 :
                        ((s_snake_db.params.cycle_cntr == 1) || (s_snake_db.params.cycle_cntr == (s_snake_db.config.snake_length-2))) ? 2 : 0;
        uint8_t strip_id;
        for (strip_id = 0; strip_id < MAX_SUPPORTED_NUM_OF_STRIPS; strip_id++)
        {
            if (s_snake_db.params.cycle_cntr < s_snake_db.config.snake_length)
            {
                frame[strip_id][0][RGB_GREEN] = s_snake_db.params.color[RGB_GREEN]>>shift;
                frame[strip_id][0][RGB_RED]   = s_snake_db.params.color[RGB_RED]>>shift;
                frame[strip_id][0][RGB_BLUE]  = s_snake_db.params.color[RGB_BLUE]>>shift;
            }
            else
            {
                frame[strip_id][0][RGB_GREEN] = 0;
                frame[strip_id][0][RGB_RED] = 0;
                frame[strip_id][0][RGB_BLUE] = 0;
            }
        }
        s_snake_db.params.cycle_cntr++;
        if (s_snake_db.params.cycle_cntr == s_snake_db.config.cycle_length)
        {
            s_snake_db.params.cycle_cntr = 0;
        }
    }
    s_snake_db.params.step_cntr++;
    if (s_snake_db.params.step_cntr == s_snake_db.config.step_size)
    {
        /* Need to find a way to hand shake between Shows mechanism and shows, accessing Global shows struct is not ideal */
        s_snake_db.show_db.next_frame = NEXT_FRAME_MOVE_FORWARD;
        s_snake_db.params.step_cntr = 0;
    }
    else
    {
        /* Need to find a way to hand shake between Shows mechanism and shows, accessing Global shows struct is not ideal */
        s_snake_db.show_db.next_frame = NEXT_FRAME_NO_CHANGE;
    }
}

/* =========================================================================================== */
/*   PUBLIC FUNCTION                                                                           */
/* =========================================================================================== */
show_db_t* snake_show_init(uint16_t show_id)
{
    /* init snake database configurations and parameters */
    snake_db_init_defaults();
    /* init snake's show database */
    s_snake_db.show_db.show_mode = SHOW_MODE_FRAME_BY_FRAME;
    s_snake_db.show_db.next_frame = NEXT_FRAME_MOVE_FORWARD;
    s_snake_db.show_db.frame_duration = 40;
    s_snake_db.show_db.fade_in_enable = true;
    s_snake_db.show_db.fade_out_enable = true;
    s_snake_db.show_db.max_power = 60;
    s_snake_db.show_db.direction = 0;
    s_snake_db.show_db.set_frame = snake_show_set_frame;
    /* return pointer to snake's show database */
    return &s_snake_db.show_db;
}
