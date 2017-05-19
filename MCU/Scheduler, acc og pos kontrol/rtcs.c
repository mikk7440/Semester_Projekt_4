/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: rtcs.c
*
* PROJECT....: EMP
*
* DESCRIPTION: See module specification file (.h-file).
*
* Change Log:
*****************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150303  MoH   Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "global_def.h"
#include "rtcs.h"


/*****************************    Defines    *******************************/

typedef struct
{
    INT8U   head;
    INT8U   tail;
    INT8U   length;
    SEM     queue_not_full;
    SEM     queue_not_empty;
    INT16U  buffer[QUEUE_SIZE];
//    INT16U  buffer[];
} queue_structure;

typedef struct
{
  INT8U  condition;
  INT8U  name;
  INT8U  state;
  INT8U  event;
  INT8U  sem;
  INT16U timer;
  void (*tf)(INT8U, INT8U, INT8U, INT8U);  // Pointer to task function
} task_structure;

typedef struct
{
  INT8U  condition;
  INT8U  type;
  INT8U  count;
}sem_structure;


/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
extern volatile INT16S ticks;

HANDLE current_task;

queue_structure     queue_pool[MAX_QUEUES];
sem_structure       semaphore_pool[MAX_SEMAPHORES];
task_structure      task_pool[MAX_TASKS];

/*****************************   Functions   *******************************/

/***************************************************************************   Queue   ****************************************************************************************************/

INT8S open_queue(INT8U id)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
    INT8S result = 0;

    if(id < MAX_QUEUES)
    {
        queue_pool[id].head = 0;
        queue_pool[id].tail = 0;
        queue_pool[id].length = 0;
        queue_pool[id].queue_not_full  = id;
        queue_pool[id].queue_not_empty = MAX_QUEUES + id;
        preset_sem( queue_pool[id].queue_not_full, QUEUE_SIZE );
//        queue_pool[id].buffer[queuesize];
        result = id;
    }else{
        result = -1;
    }

    return result;

}


BOOLEAN put_queue( INT8U id, INT16U ch, INT16U timeout )
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
  BOOLEAN result = FALSE;

  if( id < MAX_QUEUES )
  {
    if( wait_sem( queue_pool[id].queue_not_full, timeout ) )
    {
      queue_pool[id].buffer[queue_pool[id].head++] = ch;
      queue_pool[id].head &= 0b01111111;
      queue_pool[id].length++;
      signal( queue_pool[id].queue_not_empty );
      result = TRUE;
    }
  }
  return( result );
}

BOOLEAN get_queue( id, pch, timeout )
INT8U  id;
INT16U *pch;
INT16U timeout;
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
  BOOLEAN result = FALSE;

  if( id < MAX_QUEUES )
  {
    if( wait_sem( queue_pool[id].queue_not_empty, timeout ) )
    {
      *pch = queue_pool[id].buffer[queue_pool[id].tail++];
      queue_pool[id].tail &= 0b01111111;
      queue_pool[id].length--;
      signal( queue_pool[id].queue_not_full );
      result = TRUE;
    }
  }
  return( result );
}

INT16U peek_queue(INT8U id)
{
   return  queue_pool[id].buffer[queue_pool[id].tail];
}


/***************************************************************************   Semaphores   ****************************************************************************************************/

extern void wait( INT16U timeout )
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
  task_pool[current_task].timer     = timeout;
  task_pool[current_task].condition = TASK_WAIT_FOR_TIMEOUT;
}

extern BOOLEAN wait_sem( INT8U sem, INT16U timeout )
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
  BOOLEAN result = TRUE;

  if( semaphore_pool[sem].count )                          // If the count value of the semaphore has a value, then ...
  {
    semaphore_pool[sem].count--;
    task_pool[current_task].condition = TASK_READY;
    result = TRUE;
  }
  else
  {
    task_pool[current_task].sem       = sem;
    task_pool[current_task].condition = TASK_WAIT_FOR_SEMAPHORE;
    if( timeout )
    {
      task_pool[current_task].timer     = timeout;
      task_pool[current_task].condition |= TASK_WAIT_FOR_TIMEOUT;
    }
    result = FALSE;
  }
  return( result );
}

extern void signal( INT8U sem )
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
  if( sem < MAX_SEMAPHORES )
    semaphore_pool[sem].count++;
}

extern void preset_sem( INT8U sem, INT8U signals )
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
  if( sem < MAX_SEMAPHORES )
    semaphore_pool[sem].count = signals;
}

/***************************************************************************   Task   ****************************************************************************************************/

extern HANDLE start_task( INT8U name, void (*tf)(INT8U, INT8U, INT8U, INT8U) )
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
  HANDLE this_id;

  this_id = retrieve_id();
  if( this_id != ERROR_TASK )
  {
    //pot[this_id].id    = this_id;
    task_pool[this_id].condition = TASK_READY;
    task_pool[this_id].name      = name;
    task_pool[this_id].state     = 0;
    task_pool[this_id].timer     = 0;
    task_pool[this_id].tf        = tf;
  }
  return( 0 );
}


/***************************************************************************   Others   ****************************************************************************************************/

HANDLE retrieve_id(void)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
  static HANDLE next_id = 0;

  return( next_id++ );
}

void i_am_alive( INT8U my_id, INT8U my_state, INT8U event, INT8U data )
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
  if( my_state == 0 )
  {
    // Turn on the LED.
    GPIO_PORTD_DATA_R |= 0x40;
    set_state( 1 );
  }
  else
  {
    // Turn off the LED.
    GPIO_PORTD_DATA_R &= ~(0x40);
    set_state( 0 );
  }
  wait( 200 );
}

extern void set_state( INT8U new_state )
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
   task_pool[current_task].state = new_state;
}


/***************************************************************************   RTCS   ****************************************************************************************************/

extern INT8U init_rtcs()
{
  INT8U i;

  for( i = 0; i < MAX_TASKS; i++ )
  {
    task_pool[i].condition = TASK_IDLE;
  }
  start_task( SYS_TASK+1, i_am_alive );
  return( 1 );
}

void schedule()
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{

  while(1)
  {
    while( !ticks );
    ticks--;
    current_task = 0;
    do
    {
      if( task_pool[current_task].condition & TASK_WAIT_FOR_SEMAPHORE )
      {
        if( semaphore_pool[task_pool[current_task].sem].count )
        {
          if( !( task_pool[current_task].sem < (2 * MAX_QUEUES )))
          semaphore_pool[task_pool[current_task].sem].count--;
          task_pool[current_task].event     = EVENT_SIGNAL;
          task_pool[current_task].condition = TASK_READY;
        }
      }

      if( task_pool[current_task].condition & TASK_WAIT_FOR_TIMEOUT )
      {
        if( task_pool[current_task].timer )
        {
          task_pool[current_task].timer--;
          if( task_pool[current_task].timer == 0 )
          {
            task_pool[current_task].event     = EVENT_TIMEOUT;
            task_pool[current_task].condition = TASK_READY;
          }
        }
      }

      if( task_pool[current_task].condition == TASK_READY )
        task_pool[current_task].tf(current_task, task_pool[current_task].state, 0, 0);
      current_task++;
    } while ( task_pool[current_task].condition != TASK_IDLE );
  }
}
/****************************** End Of Module *******************************/







