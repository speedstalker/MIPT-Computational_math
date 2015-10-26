/*
 * This program generates tasks for my implementation of GMRES;
 * tasks have the following structure (all elements in a column):
 * maximum calculation error, size of matrix, column of free terms and then
 * the first line of the matrix, the second and so on.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <errno.h>
#include <assert.h>
#include <limits.h>


//------------------------------------------------------------------------------
// Error handling macroses
//------------------------------------------------------------------------------
#define IS_DEBUG 1

#if (IS_DEBUG == 1)
//{
   #define HANDLE_ERROR HANDLE_ERROR_wL
   #define HANDLE_ERROR_wL(msg)                                                    \
                   do                                                              \
                   {                                                               \
                   char err_msg[256] = {0};                                        \
                                                                                   \
                   snprintf (err_msg, 255, "%d. " msg "%c", __LINE__, '\0');       \
                   perror (err_msg);                                               \
                   exit   (EXIT_FAILURE);                                          \
                   }                                                               \
                   while (0)
//}
#else
//{
   #define HANDLE_ERROR_wL HANDLE_ERROR
   #define HANDLE_ERROR(msg) \
                  do { perror(msg); exit(EXIT_FAILURE); } while (0)
//}
#endif
//------------------------------------------------------------------------------

#define ABS_OF_VALUE_LIMIT 100


int rand_int_wo_bias (int limit);

int main (int argc, char* argv[])
{
char*  str_calc_error  = NULL;
double calc_error      = 0;

char* str_size_of_matrix = NULL;
long  size_of_matrix     = 0;

char* endptr = NULL;

int i = 0;
int cur_numb = 0;

//------------------------------------------------------------------------------
// Getting size of matrix and calc_error
//------------------------------------------------------------------------------
if (argc != 3)
        {
        printf ("Usage: %s CALCULATION_ERROR SIZE_OF_MATRIX\n", argv[0]);
        exit (EXIT_FAILURE);
        }
//------------------------------------------------------------------------------
str_calc_error = argv[1];

endptr = NULL;
errno = 0;
calc_error = strtod (str_calc_error, &endptr);
if (errno != 0)
        HANDLE_ERROR ("strtol calc_error");

if ((endptr == str_calc_error) || (*endptr != '\0'))
        {
        printf ("CALCULATION_ERROR must contain only digits!\n");
        exit (EXIT_FAILURE);
        }

if (calc_error <= 0)
        {
        printf ("CALCULATION_ERROR must be strictly positive number!\n");
        exit (EXIT_FAILURE);
        }
//------------------------------------------------------------------------------
str_size_of_matrix = argv[2];

errno = 0;
size_of_matrix = strtol (str_size_of_matrix, &endptr, 10);    // 10 - base of numerical system
if ((errno == ERANGE && (size_of_matrix == LONG_MAX || size_of_matrix == LONG_MIN))
                   || (errno != 0 && size_of_matrix == 0))
        HANDLE_ERROR ("strtol size_of_matrix");

if ((endptr == str_size_of_matrix) || (*endptr != '\0'))
        {
        printf ("SIZE_OF_MATRIX must contain only digits!\n");
        exit (EXIT_FAILURE);
        }

if (size_of_matrix <= 0)
        {
        printf ("SIZE_OF_MATRIX must be strictly positive number!\n");
        exit (EXIT_FAILURE);
        }
//------------------------------------------------------------------------------

printf ("%g\n", calc_error);
printf ("%ld\n", size_of_matrix);

srand (time (NULL));
for (i = 0; i < (size_of_matrix * (size_of_matrix + 1)); i++)
        {
        cur_numb = rand_int_wo_bias (2 * ABS_OF_VALUE_LIMIT) - ABS_OF_VALUE_LIMIT;
        printf ("%d\n", cur_numb);
        }

return 0;
}


// let's generate a pseudo-random number w/o modulo bias from 0 to 'limit' inclusive
// http://stackoverflow.com/questions/2999075/generate-a-random-number-within-range/2999130#2999130
inline int rand_int_wo_bias (int limit)
{
assert (limit > 0);

int ret_val = 0;
int numb_of_parts = 0;

numb_of_parts = RAND_MAX / (limit + 1);

do
        {
        ret_val = rand() / numb_of_parts;
        }
while (ret_val > limit);

return ret_val;
}



