#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <assert.h>


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

#define YES 1
#define NO  0


typedef struct vector   // you could use 1D Matrix, but it will cause inconveniencies
        {               // with dereferencing double pointers
        double* vector_ptr;

        int size_of_vector;
        } Vector;

typedef struct matrix
        {
        int** matrix_ptr;

        int numb_of_lines;
        int numb_of_columns;

        int size_of_matrix;         // if it is square matrix
        } Matrix;

int multiply_scalar_on_vector (double scalar, Vector* vector_ptr);
int sum_of_vectors (Vector* vector_1_ptr, const Vector* vector_2_ptr, int is_substraction);
double scalar_multiplication_of_vectors (const Vector* vector_1_ptr, const Vector* vector_2_ptr);

int transpose_matrix (Matrix* matrix);
int multiply_matrix_on_vector (const Matrix* matrix_ptr, const Vector* vector_ptr, Vector* result_vector_ptr);
int multiply_matrix_on_matrix (const Matrix* first_matrix_ptr, const Matrix* second_matrix_ptr, Matrix* result_matrix_ptr);

int main (int argc, char* argv[])
{
FILE* task_file = NULL;

double calc_error     = 0, calc_error_squared = 0;;
int    size_of_matrix = 0; // I know, that from task_gen it should be 'long',
                           //      not 'int', but whatever...=)

int i = 0, j = 0;
int cur_line = 0, cur_column = 0;
int ret_val = 0;

Vector vector_of_free_terms = {0}, mult_vector = {0};
Matrix matrix = {0}, transpon_matrix = {0}, mult_matrix = {0};

Vector approx_solution = {0}; // zero_approx = 0
Vector residual        = {0}; // residual = nevyazka (rus) =)
double iteration_param =  0;

Vector tmp_vector = {0};
double residual_norm = 0;

//------------------------------------------------------------------------------
// Getting the file path
//------------------------------------------------------------------------------
if (argc != 2)
        {
        printf ("Usage: %s PATH_TO_TASK_FILE\n", argv[0]);
        exit (EXIT_FAILURE);
        }
//------------------------------------------------------------------------------
// Get the calc_error and size_of_matrix
//------------------------------------------------------------------------------
errno = 0;
if ((task_file = fopen (argv[1], "r")) == NULL)
        HANDLE_ERROR ("task_file fopen");

errno = 0;
if (((ret_val = fscanf(task_file, "%lg", &calc_error)) <= 0) || (errno != 0))
        HANDLE_ERROR ("calc_error fscanf");
printf ("calc_error = %g\n", calc_error);

errno = 0;
if (((ret_val = fscanf(task_file, "%d", &size_of_matrix)) <= 0) || (errno != 0))
        HANDLE_ERROR ("size_of_matrix fscanf");
printf ("size_of_matrix = %d\n", size_of_matrix);

vector_of_free_terms.size_of_vector = size_of_matrix;

matrix.numb_of_lines   = size_of_matrix;
matrix.numb_of_columns = size_of_matrix;
matrix.size_of_matrix  = size_of_matrix;

transpon_matrix.numb_of_lines   = size_of_matrix;
transpon_matrix.numb_of_columns = size_of_matrix;
transpon_matrix.size_of_matrix  = size_of_matrix;

mult_matrix.numb_of_lines   = size_of_matrix;
mult_matrix.numb_of_columns = size_of_matrix;
mult_matrix.size_of_matrix  = size_of_matrix;
//------------------------------------------------------------------------------
// Allocate an 1D array for 'vector_of_free_terms', an 2D array for 'matrix',
// another 2D array for 'transponed matrix' and for 'mult_matrix' ((A^T)*T)
//------------------------------------------------------------------------------
vector_of_free_terms.vector_ptr = (double*) calloc (vector_of_free_terms.size_of_vector, sizeof (double));
assert (vector_of_free_terms.vector_ptr != NULL);

matrix.matrix_ptr = (int**) calloc (matrix.size_of_matrix, sizeof (int*));
assert (matrix.matrix_ptr != NULL);
for (i = 0; i < matrix.size_of_matrix; i++)
        {
        matrix.matrix_ptr[i] = (int*) calloc (matrix.size_of_matrix, sizeof (int));
        assert (matrix.matrix_ptr[i] != NULL);
        }

transpon_matrix.matrix_ptr = (int**) calloc (transpon_matrix.size_of_matrix, sizeof (int*));
assert (transpon_matrix.matrix_ptr != NULL);
for (i = 0; i < transpon_matrix.size_of_matrix; i++)
        {
        transpon_matrix.matrix_ptr[i] = (int*) calloc (transpon_matrix.size_of_matrix, sizeof (int));
        assert (transpon_matrix.matrix_ptr[i] != NULL);
        }

mult_matrix.matrix_ptr = (int**) calloc (mult_matrix.size_of_matrix, sizeof (int*));
assert (mult_matrix.matrix_ptr != NULL);
for (i = 0; i < mult_matrix.size_of_matrix; i++)
        {
        mult_matrix.matrix_ptr[i] = (int*) calloc (mult_matrix.size_of_matrix, sizeof (int));
        assert (mult_matrix.matrix_ptr[i] != NULL);
        }
//------------------------------------------------------------------------------
// Copy all the data from file to our arrays (and in transponed matrix)
//      and close the file stream
//------------------------------------------------------------------------------
for (i = 0; i < vector_of_free_terms.size_of_vector; i++)
        if (((ret_val = fscanf (task_file, "%lg", &vector_of_free_terms.vector_ptr[i])) <= 0) || (errno != 0))
                HANDLE_ERROR ("vector_of_free_terms.vector_ptr fscanf");

for (cur_line = 0; cur_line < matrix.size_of_matrix; cur_line++)
        for (cur_column = 0; cur_column < matrix.size_of_matrix; cur_column++)
                {
                ret_val = fscanf (task_file, "%d", &matrix.matrix_ptr[cur_line][cur_column]);
                if ((ret_val <= 0) || errno != 0)
                        HANDLE_ERROR ("matrix.matrix_ptr fscanf");
                transpon_matrix.matrix_ptr[cur_line][cur_column] = matrix.matrix_ptr[cur_line][cur_column];
                }

errno = 0;
if (fclose (task_file) != 0)
        HANDLE_ERROR ("task_file fclose");
//------------------------------------------------------------------------------
// Get 'transpon_matrix' and multiply 'mult_matrix' = ((A^T)*A)
//------------------------------------------------------------------------------
for (i = 0; i < matrix.size_of_matrix; i++)
        {
        for (j = 0; j < matrix.size_of_matrix; j++)
                printf ("\t%d\t", matrix.matrix_ptr[i][j]);
        printf ("\n");
        }

printf ("\n");

if (transpose_matrix (&transpon_matrix) != 0)
        {
        printf ("error in transpose_matrix\n");
        exit (EXIT_FAILURE);
        }

for (i = 0; i < transpon_matrix.size_of_matrix; i++)
        {
        for (j = 0; j < transpon_matrix.size_of_matrix; j++)
                printf ("\t%d\t", transpon_matrix.matrix_ptr[i][j]);
        printf ("\n");
        }

printf ("\n");

if (multiply_matrix_on_matrix (&transpon_matrix, &matrix, &mult_matrix) != 0)
        {
        printf ("error in multiply_matrix_on_matrix\n");
        exit (EXIT_FAILURE);
        }

for (i = 0; i < mult_matrix.size_of_matrix; i++)
        {
        for (j = 0; j < mult_matrix.size_of_matrix; j++)
                printf ("\t%d\t", mult_matrix.matrix_ptr[i][j]);
        printf ("\n");
        }

printf ("\n");
//------------------------------------------------------------------------------
// Multiply (A^T) on 'vector_of_free_terms'
//------------------------------------------------------------------------------
for (i = 0; i < vector_of_free_terms.size_of_vector; i++)
        {
        printf ("\t%lg", vector_of_free_terms.vector_ptr[i]);
        printf ("\n");
        }

printf ("\n");

mult_vector.size_of_vector = vector_of_free_terms.size_of_vector;
mult_vector.vector_ptr = (double*) calloc (mult_vector.size_of_vector, sizeof (double));
assert (mult_vector.vector_ptr != NULL);

if (multiply_matrix_on_vector (&transpon_matrix, &vector_of_free_terms, &mult_vector) != 0)
        {
        printf ("error in multiply_matrix_on_vector\n");
        exit (EXIT_FAILURE);
        }

for (i = 0; i < mult_vector.size_of_vector; i++)
        {
        printf ("\t%lg", mult_vector.vector_ptr[i]);
        printf ("\n");
        }

printf ("\n");
//------------------------------------------------------------------------------
// Let's implement the main algorithm
//------------------------------------------------------------------------------
residual.size_of_vector = matrix.size_of_matrix;
residual.vector_ptr = (double*) calloc (residual.size_of_vector, sizeof (double));
assert (residual.vector_ptr != NULL);

approx_solution.size_of_vector = matrix.size_of_matrix;
approx_solution.vector_ptr = (double*) calloc (approx_solution.size_of_vector, sizeof (double));
assert (approx_solution.vector_ptr != NULL);

for (i = 0; i < approx_solution.size_of_vector; i++)
        approx_solution.vector_ptr[i] = 0;     // zero approximation will be 0

tmp_vector.size_of_vector = matrix.size_of_matrix;
tmp_vector.vector_ptr = (double*) calloc (tmp_vector.size_of_vector, sizeof (double));
assert (tmp_vector.vector_ptr != NULL);

calc_error_squared = calc_error * calc_error;

do
        {
        //------------------------------------------------------------------------------
        // calculation of the 'residual'
        //------------------------------------------------------------------------------
        // 1st part
        if (multiply_matrix_on_vector (&mult_matrix, &approx_solution, &residual) != 0)
                {
                printf ("error in residual multiply_matrix_on_vector\n");
                exit (EXIT_FAILURE);
                }

        for (i = 0; i < residual.size_of_vector; i++)
                {
                printf ("\t%lg", residual.vector_ptr[i]);
                printf ("\n");
                }

        printf ("\n");

        // 2nd part
        if (sum_of_vectors (&residual, &mult_vector, YES) != 0)
                {
                printf ("error in residual multiply_matrix_on_vector\n");
                exit (EXIT_FAILURE);
                }

        for (i = 0; i < residual.size_of_vector; i++)
                {
                printf ("\t%lg", residual.vector_ptr[i]);
                printf ("\n");
                }

        printf ("\n");
        //------------------------------------------------------------------------------
        // calculation of the 'iteration_param'
        //------------------------------------------------------------------------------
        if (multiply_matrix_on_vector (&mult_matrix, &residual, &tmp_vector) != 0)
                {
                printf ("error in iteration_param multiply_matrix_on_vector\n");
                exit (EXIT_FAILURE);
                }

        iteration_param  = scalar_multiplication_of_vectors (&tmp_vector, &residual);
        iteration_param /= scalar_multiplication_of_vectors (&tmp_vector, &tmp_vector);
        printf ("%lg\n", iteration_param);
        printf ("\n");
        //------------------------------------------------------------------------------
        // calculation of the next 'approx_solution'
        //------------------------------------------------------------------------------
        if (multiply_scalar_on_vector (iteration_param, &residual) != 0)
                {
                printf ("error in multiply_scalar_on_vector\n");
                exit (EXIT_FAILURE);
                }

        for (i = 0; i < residual.size_of_vector; i++)
                {
                printf ("\t%lg", residual.vector_ptr[i]);
                printf ("\n");
                }

        printf ("\n");

        if (sum_of_vectors (&approx_solution, &residual, NO) != 0)
                {
                printf ("error in residual multiply_matrix_on_vector\n");
                exit (EXIT_FAILURE);
                }

        for (i = 0; i < approx_solution.size_of_vector; i++)
                {
                printf ("\t%lg", approx_solution.vector_ptr[i]);
                printf ("\n");
                }

        printf ("\n");
        //------------------------------------------------------------------------------
        // calculation of residual norm (in order to determine when to stop calculation cycle),
        //      I use the third norm: Euclidean norm (but I squared the calc_error,
        //      rather than square_rooted the norm)
        //------------------------------------------------------------------------------
        residual_norm = scalar_multiplication_of_vectors (&residual, &residual);
        //------------------------------------------------------------------------------
        }
        while (residual_norm > calc_error_squared);


//------------------------------------------------------------------------------
// Printing the answer
//------------------------------------------------------------------------------
printf ("\nThe answer is:\n");
for (i = 0; i < approx_solution.size_of_vector; i++)
        {
        printf ("\t%lg\n", approx_solution.vector_ptr[i]);
        }

printf ("\n");
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Cleanup
//------------------------------------------------------------------------------


free (tmp_vector.vector_ptr);

free (approx_solution.vector_ptr);

free (residual.vector_ptr);

free (mult_vector.vector_ptr);

for (i = 0; i < mult_matrix.size_of_matrix; i++)
        free (mult_matrix.matrix_ptr[i]);
free (mult_matrix.matrix_ptr);

for (i = 0; i < transpon_matrix.size_of_matrix; i++)
        free (transpon_matrix.matrix_ptr[i]);
free (transpon_matrix.matrix_ptr);

for (i = 0; i < matrix.size_of_matrix; i++)
        free (matrix.matrix_ptr[i]);
free (matrix.matrix_ptr);

free (vector_of_free_terms.vector_ptr);
//------------------------------------------------------------------------------

return 0;
}


int multiply_scalar_on_vector (double scalar, Vector* vector_ptr)
{
int i = 0;
Vector vector = *vector_ptr;

assert ((vector_ptr != 0) && (vector_ptr->vector_ptr != 0));

for (i = 0; i < vector.size_of_vector; i++)
        vector.vector_ptr[i] *= scalar;

*vector_ptr = vector;

return 0;
}

int sum_of_vectors (Vector* vector_1_ptr, const Vector* vector_2_ptr, int is_substraction)
{
int i = 0;

      Vector vector_1 = *vector_1_ptr;
const Vector vector_2 = *vector_2_ptr;

assert ((vector_1_ptr != 0) && (vector_2_ptr != 0));
assert ((vector_1_ptr->vector_ptr != 0) && (vector_2_ptr->vector_ptr != 0));
assert ((vector_1_ptr->size_of_vector) == (vector_2_ptr->size_of_vector));

for (i = 0; i < vector_1.size_of_vector; i++)
        if (is_substraction != 0)
                vector_1.vector_ptr[i] -= vector_2.vector_ptr[i];
        else
                vector_1.vector_ptr[i] += vector_2.vector_ptr[i];

*vector_1_ptr = vector_1;

return 0;
}

double scalar_multiplication_of_vectors (const Vector* vector_1_ptr, const Vector* vector_2_ptr)
{
int i = 0;
double result = 0;

const Vector vector_1 = *vector_1_ptr;
const Vector vector_2 = *vector_2_ptr;

assert ((vector_1_ptr != 0) && (vector_2_ptr != 0));
assert ((vector_1_ptr->vector_ptr != 0) && (vector_2_ptr->vector_ptr != 0));
assert ((vector_1_ptr->size_of_vector) == (vector_2_ptr->size_of_vector));

for (i = 0; i < vector_1.size_of_vector; i++)
        result += (vector_1.vector_ptr[i] * vector_2.vector_ptr[i]);

return result;
}

int transpose_matrix (Matrix* matrix)
{
int cur_line = 0, cur_column = 0;

int column_to_begin = 1;
int tmp_numb = 0;

assert ((matrix != 0) && (matrix->matrix_ptr != 0));
assert (   (matrix->size_of_matrix == matrix->numb_of_lines)
        && (matrix->size_of_matrix == matrix->numb_of_columns)
        && (matrix->size_of_matrix > 0));

for (cur_line = 0; cur_line < (matrix->size_of_matrix - 1); cur_line++)
        {
        for (cur_column = column_to_begin; cur_column < matrix->size_of_matrix; cur_column++)
                {
                tmp_numb = matrix->matrix_ptr[cur_line][cur_column];
                matrix->matrix_ptr[cur_line][cur_column] = matrix->matrix_ptr[cur_column][cur_line];
                matrix->matrix_ptr[cur_column][cur_line] = tmp_numb;
                }
        column_to_begin++;
        }

return 0;
}

int multiply_matrix_on_vector (const Matrix* matrix_ptr, const Vector* vector_ptr, Vector* result_vector_ptr)
{
int i = 0;
int cur_line = 0;

const Matrix matrix = *matrix_ptr;
const Vector vector = *vector_ptr;
Vector res_vector = *result_vector_ptr;

assert ((matrix_ptr != 0) && (vector_ptr != 0) && (result_vector_ptr != 0));
assert ((matrix_ptr->matrix_ptr != 0) && (vector_ptr->vector_ptr != 0));
assert (matrix_ptr->numb_of_columns == vector_ptr->size_of_vector);


res_vector.size_of_vector  = matrix.numb_of_lines;

for (cur_line = 0; cur_line < res_vector.size_of_vector; cur_line++)
        {
        res_vector.vector_ptr[cur_line] = 0;

        for (i = 0; i < (matrix.numb_of_columns); i++)
                res_vector.vector_ptr[cur_line] +=
                        matrix.matrix_ptr[cur_line][i] * vector.vector_ptr[i];
        }

*result_vector_ptr = res_vector;

return 0;
}

int multiply_matrix_on_matrix (const Matrix* first_matrix_ptr, const Matrix* second_matrix_ptr, Matrix* result_matrix_ptr)
{
int i = 0;
int cur_line = 0, cur_column = 0;

const Matrix matrix_1 = *first_matrix_ptr, matrix_2 = *second_matrix_ptr;
Matrix res_matrix = *result_matrix_ptr;

assert ((first_matrix_ptr != 0) && (second_matrix_ptr != 0) && (result_matrix_ptr != 0));
assert ((first_matrix_ptr->matrix_ptr != 0) && (second_matrix_ptr->matrix_ptr != 0));
assert (first_matrix_ptr->numb_of_columns == second_matrix_ptr->numb_of_lines);


if (   (matrix_1.numb_of_lines   == matrix_2.numb_of_lines)
    && (matrix_1.numb_of_columns == matrix_2.numb_of_columns)
    && (matrix_1.numb_of_lines   == matrix_1.numb_of_columns))
        {
        res_matrix.size_of_matrix  = matrix_1.size_of_matrix;
        res_matrix.numb_of_lines   = matrix_1.size_of_matrix;
        res_matrix.numb_of_columns = matrix_1.size_of_matrix;
        }
else
        {
        res_matrix.size_of_matrix  = 0;
        res_matrix.numb_of_lines   = matrix_1.numb_of_lines;
        res_matrix.numb_of_columns = matrix_2.numb_of_columns;
        }

for (cur_line = 0; cur_line < res_matrix.numb_of_lines; cur_line++)
        {
        for (cur_column = 0; cur_column < res_matrix.numb_of_columns; cur_column++)
                {
                res_matrix.matrix_ptr[cur_line][cur_column] = 0;

                for (i = 0; i < (matrix_1.numb_of_columns); i++)
                        res_matrix.matrix_ptr[cur_line][cur_column] +=
                                matrix_1.matrix_ptr[cur_line][i] * matrix_2.matrix_ptr[i][cur_column];
                }
        }

*result_matrix_ptr = res_matrix;

return 0;
}

