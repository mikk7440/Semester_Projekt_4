/*****************************************************************************
* University of Southern Denmark
* Embedded C Programming (ECP)
*
* MODULENAME.: sem.c
*
* PROJECT....: ECP
*
* DESCRIPTION: See module specification file (.h-file).
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 090315  MoH   Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include "emp_type.h"
#include "sem.h"
#include "global_def.h"
/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

INT8U    semaphore_pool[MAX_SEM];

/*****************************   Functions   *******************************/

 BOOLEAN wait_1(INT8U sem_no)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
  INT8U Result;
  
  if( sem_no < MAX_SEM )
  {
	  Result = semaphore_pool[sem_no];
	  semaphore_pool[sem_no] = FALSE;
  }
  else
      Result = FALSE;
  return( Result );
}

void signal_1(INT8U sem_no)
/*****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
  if( sem_no < MAX_SEM )
	  semaphore_pool[sem_no] = TRUE;
}

/****************************** End Of Module *******************************/












