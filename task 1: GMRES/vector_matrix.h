#include <assert.h>

#include "vector_matrix.h"


//==============================================================================
// Realisation of functions
//==============================================================================
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
//==============================================================================

