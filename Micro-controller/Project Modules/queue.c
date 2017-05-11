/*****************************************************************************
* Odense University College of Enginerring
* Embedded C Programming (ECP)
*
* MODULENAME.: queue.c
*
* PROJECT....: ECP
*
* DESCRIPTION:
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 090928  MoH   Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/

#include "emp_type.h"
#include "global_def.h"
#include "queue.h"

/*****************************    Defines    *******************************/
#define QUEUE_SIZE          128
#define NUMBER_OF_QUEUES    8

typedef struct 
{
	INT8U	head;
	INT8U	tail;
	INT8U	length;
	INT16U  buffer[QUEUE_SIZE];
} queue_structure;

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
queue_structure queue_pool[NUMBER_OF_QUEUES];

/*****************************   Functions   *******************************/


INT8S queue_open(INT8U id)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
    INT8S result = 0;

    if(id < NUMBER_OF_QUEUES)
    {
        queue_pool[id].head = 0;
        queue_pool[id].tail = 0;
        queue_pool[id].length = 0;
        result = id;
    }else{
        result = -1;
    }

    return result;

}

INT8U queue_put(INT8U id, INT16U ch)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
    INT8U result = FALSE;

    if(id < NUMBER_OF_QUEUES)
    {
        if (queue_pool[id].length < QUEUE_SIZE)
        {
            queue_pool[id].buffer[queue_pool[id].head++] = ch;
            queue_pool[id].head &= 0b01111111;
            queue_pool[id].length++;
            result = TRUE;
        }
    }

    return result;
}

INT16U queue_get(INT8U id)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
    INT16U Ch = '/0';

    if(id < NUMBER_OF_QUEUES)
    {
        if(queue_pool[id].length)
        {
            Ch = queue_pool[id].buffer[queue_pool[id].tail++];
            queue_pool[id].tail &= 0b01111111;
            queue_pool[id].length--;
        }
    }

    return Ch;
}

/****************************** End Of Module *******************************/












