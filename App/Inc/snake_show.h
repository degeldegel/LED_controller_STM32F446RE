/**
  ******************************************************************************
  * @file           : snake_show.h
  * @brief          : header file of snake show
  ******************************************************************************
  *
  ******************************************************************************
  */

#ifndef _SNAKE_SHOW_H
#define _SNAKE_SHOW_H

/* =========================================================================================== */
/*   DEFINES AND ENUMERATORS                                                                   */
/* =========================================================================================== */

/* =========================================================================================== */
/*   STRUCTS AND TYPE DEFINITIONS                                                                               */
/* =========================================================================================== */
typedef struct snake_show_config_db
{
    uint16_t snake_length; /* length of snake in a cycle */
    uint16_t cycle_length; /* length of cycle including the snake */
    uint16_t step_size;    /* how many frames in one step - controls speed of snake */
} snake_show_config_db_t;

typedef struct snake_show_params_db
{
    uint16_t step_cntr;                 /* step counter, advanced by one each step, zeroed every step_size. */
    uint16_t cycle_cntr;                /* cycle counter, advanced by one each step of the snake, zeroed every cycle_length */
    uint8_t  color[MAX_RGB_PER_LED];    /* current color */
} snake_show_params_db_t;

typedef struct snake_show_db
{
    snake_show_config_db_t config;  /* snake configuration */
    snake_show_params_db_t params;  /* snake parameters */
} snake_show_db_t;

/* =========================================================================================== */
/*   MACROS                                                                                    */
/* =========================================================================================== */

/* =========================================================================================== */
/*   PUBLIC FUNCTION DECLARATION                                                               */
/* =========================================================================================== */

/* =========================================================================================== */
/*   USER SHOWS DEFINITIONS AND DECLARATIONS                                                   */
/* =========================================================================================== */


#endif  /* _SNAKE_SHOW_H */

