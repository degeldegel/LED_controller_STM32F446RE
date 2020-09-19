#include "stm32f4xx_hal.h"
#include "scheduler.h"
#include "shows.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "porting_layer.h"

/* =========================================================================================== */
/*   PRIVATE DEFINES                                                                           */
/* =========================================================================================== */
#define SCHEDULER_TASK_STACK_SIZE       (128)
#define SCHEDULER_TASK_PRIO             (configMAX_PRIORITIES - 2)

/* =========================================================================================== */
/*   GLOBAL VARIABLES                                                                          */
/* =========================================================================================== */
TaskHandle_t gp_scheduler_task_handler = NULL;
SemaphoreHandle_t gp_run_scheduler_sem = NULL;

/* =========================================================================================== */
/*   STATIC VARIABLES                                                                          */
/* =========================================================================================== */
uint32_t s_frame_idx = 0;

/* =========================================================================================== */
/*   PRIVATE FUNCTION                                                                          */
/* =========================================================================================== */



//extern volatile show_db_t shows[NUM_OF_SHOWS];
//extern TIM_HandleTypeDef htim3;
//
//volatile scheduler_db_t schdlr_db;
//
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//  if (htim == &htim3)
//  {
//      uint16_t show_time = scheduler_timer_handler();
//      __HAL_TIM_SET_AUTORELOAD(&htim3, show_time);
//  }
//}
//
//uint16_t scheduler_timer_handler(void)
//{
//
//    uint8_t curr_showID = schdlr_db.show[schdlr_db.current_show].showId;
//    uint8_t found_new_show = FALSE;
//    uint8_t all_shows_finished = TRUE;
//    uint8_t new_time_sec = 10;
//    uint8_t i;
//
//    /* stop current running show */
//    shows[curr_showID].status = SHOW_STATUS_STOP;
//
//    /* check if not all shows run already */
//    for (i=0; i<schdlr_db.num_of_shows; i++)
//    {
//        if (schdlr_db.show[i].played == SCHDLR_SHOW_DIDNT_PLAY)
//        {
//            all_shows_finished = FALSE;
//        }
//    }
//    /* if all shows run, zero the database */
//    if (all_shows_finished)
//    {
//        for (i=0; i<schdlr_db.num_of_shows; i++)
//        {
//            schdlr_db.show[i].played = SCHDLR_SHOW_DIDNT_PLAY;
//        }
//    }
//    /* choose new show */
//    srand(SysTick->VAL);
//    while (!found_new_show)
//    { /* loop till find new random show that wasn't run yet */
//        uint8_t rand_new_show_idx = rand()%schdlr_db.num_of_shows;
//        if (schdlr_db.show[rand_new_show_idx].played == SCHDLR_SHOW_DIDNT_PLAY)
//        {
//            schdlr_db.current_show = rand_new_show_idx;
//            schdlr_db.show[rand_new_show_idx].played = SCHDLR_SHOW_PLAYED;
//            found_new_show = TRUE;
//        }
//    }
//    /* activate next show */
//    curr_showID = schdlr_db.show[schdlr_db.current_show].showId;
//    shows[curr_showID].status = SHOW_STATUS_START;
//    /* choose new random run time */
//    new_time_sec = (rand()%(schdlr_db.maximum_time_run - schdlr_db.minimum_time_run)) + schdlr_db.minimum_time_run;
//    /* return new time in ticks of timer3 */
//    return new_time_sec*500;
//}
//

/**
  * @brief      init scheduler function
  * @param      void
  * @retval     void
  * @details    initialize scheduler database and prepares scheduler for first run
  */
void init_scheduler(void)
{
    //TODO
//    uint8_t show_idx;
//    schdlr_db.current_show = 0;
//    schdlr_db.num_of_shows = 5;
//    schdlr_db.maximum_time_run = 40;
//    schdlr_db.minimum_time_run = 20;
//    for (show_idx=0; show_idx<schdlr_db.num_of_shows; show_idx++)
//    {
//        schdlr_db.show[show_idx].showId = show_idx;
//        schdlr_db.show[show_idx].played = SCHDLR_SHOW_DIDNT_PLAY;
//    }
    pl_start_frame_timer();
}

/**
  * @brief      Main scheduler task
  * @param      void
  * @param      void* additional arguments passed to task (NULL for this funciton)
  * @retval     void
  * @details    scheduler task implementation
  */
void scheduler_task(void* p_argument)
{
    BaseType_t result;
    init_scheduler();
    while (true)
    {
        result = xSemaphoreTake(gp_run_scheduler_sem, portMAX_DELAY);
        PL_ASSERT_COND(result);
        run_show(0, s_frame_idx);
        s_frame_idx++;
    }
}

/* =========================================================================================== */
/*   PUBLIC FUNCTION                                                                           */
/* =========================================================================================== */
/**
  * @brief      activates scheduler task run
  * @param      void
  * @retval     void
  * @details    This function activates a scheduler run by giving semaphor
  */
void run_scheduler(void)
{
    xSemaphoreGive(gp_run_scheduler_sem);
}

/**
  * @brief      init freeRTOS resources related to the scheduler
  * @param      void
  * @retval     void
  * @details    initializes all resources related to scheduler
  */
void init_scheduler_rtos_resources(void)
{
    /* scheduler Semaphore */
    gp_run_scheduler_sem = xSemaphoreCreateBinary();

    /* check Semaphores creation success */
    PL_ASSERT_COND(gp_run_scheduler_sem);

    /* adding Semaphores for debug */
    vQueueAddToRegistry(gp_run_scheduler_sem, "Run scheduler Sem");
}

/**
  * @brief      init freeRTOS tasks related to the scheduler
  * @param      void
  * @retval     void
  * @details    initializes all tasks related to scheduler
  */
void init_scheduler_tasks(void)
{
    /* Init scheduler task */
    BaseType_t res;
    res = xTaskCreate(scheduler_task,                   // The task implementation
                      "scheduler main task",            // The task text name
                      SCHEDULER_TASK_STACK_SIZE,        // The task stack size
                      NULL,                         // Optional Parameter
                      SCHEDULER_TASK_PRIO,              // The task priority
                      &gp_scheduler_task_handler);      // The task handle
    PL_ASSERT_COND(pdPASS == res);
}
