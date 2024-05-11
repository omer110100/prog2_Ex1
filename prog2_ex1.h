#ifndef __PROG2_EX1_H__
#define __PROG2_EX1_H__

#include <stdio.h>

#define DAYS_IN_WEEK 7
#define MAX_LEN 256
#define MAX_WORKERS 100
#define HOURS_PER_SHIFT 8

/*
 * This file contains the function prototypes for the functions you need to
 * implement for the first exercise of programming2 course.
 * You should not change this file.
 *
 * Author:  David Avdat
 */

/******************************************************************************
 * enum hr_result
 */ 
typedef enum hr_result {
    SUCCESS,
    INVALID_ARGUMENTS,
    CANNOT_OPEN_FILE,
    WORKERS_OVERFLOW,
    INVALID_WORKER_ID,
    INVALID_WAGE,
    INVALID_ROLE,
    INVALID_NUM_OF_SHIFTS,
    WORKER_ALREADY_EXISTS,
    INVALID_SHIFT_DAY,
    INVALID_SHIFT_TYPE,
    WORKER_DOESNT_EXIST,
    SHIFTS_OVERFLOW,
    SHIFT_ALREADY_EXISTS,
    NO_WORKERS,
    NO_SHIFTS

} hr_result;

/******************************************************************************
 * enum worker_role
 */
typedef enum worker_role_e {
    BARTENDER,
    WAITER,
    MANAGER,
    CASHIER,
    CHEF,
    DISHWASHER
} worker_role;

/******************************************************************************
 * enum day
 */
typedef enum shift_day_e {
    SUNDAY = 1,
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY
} shift_day;

/******************************************************************************
 * shift_type
 */
typedef enum shift_type_e {
    MORNING = 0x1,
    AFTERNOON = 0x2,
    EVENING = 0x4,
    NIGHT = 0x8
} shift_type;


/******************************************************************************
 * Function: prog2_report_error_message
 * Description: This function prints an error message to the standard error
 *             output.
 * Parameters:
 *     result - the error code - a value from the enum error_code.
 * Returns: Nothing.
*/
void prog2_report_error_message(hr_result result);


/******************************************************************************
 * Function: prog2_report_worker
 * Description: This function prints worker details to a given output stream.
 * Parameters:
 *    out - the output stream
 *    id - the worker id
 *    name - the worker name
 *    wage - the worker hourly wage
 *    role - the worker role
 *    total_payment - The amount of money the worker gets for his job this week.
 * Returns: Nothing.
 */ 
 void prog2_report_worker(FILE *out, long int id, const char *name, float wage, worker_role role, float total_payment);


 /******************************************************************************
  * Function: prog2_report_shift
  * Description: This function prints shift details to a given output stream.
  * Parameters:
  *   out - the output stream
  *   day - the day of the shift
  *   type - the type of the shift
  * Returns: Nothing.
  */
 void prog2_report_shift(FILE *out, shift_day day, shift_type type);



/******************************************************************************
 * Function: prog2_report_shift_details
 * Description: This function prints shift details to a given output stream.
 * Parameters:
 *    out - the output stream
 *    day - the day of the shift
 *    type - the type of the shift
 *    num_of_workers - the number of workers that worked in this shift
 *    total_payment - the total payment for all the workers in this shift
 * Returns: Nothing.
 */

void prog2_report_shift_details(FILE *out, shift_day day, shift_type type, int num_of_workers, float total_payment);

#endif /* __PROG2_EX1_H__ */