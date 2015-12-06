#ifndef __VECTOR_MATRIX_H_INCLUDED
#define __VECTOR_MATRIX_H_INCLUDED


// defines for 'sum_of_vectors' func, for 'is_substraction' arg
#define YES 1
#define NO  0


//==============================================================================
// Declaring of structures
//==============================================================================
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
//==============================================================================


//==============================================================================
// Prototypes of functions
//==============================================================================
// RETURN VALUE:  0 - if success
int multiply_scalar_on_vector (double scalar, Vector* vector_ptr);
int sum_of_vectors                      (      Vector* vector_1_ptr, const Vector* vector_2_ptr, int is_substraction);
// RETURN VALUE:  result of scalar multiplication
double scalar_multiplication_of_vectors (const Vector* vector_1_ptr, const Vector* vector_2_ptr);

// RETURN VALUE:  0 - if success
int transpose_matrix (Matrix* matrix);
int multiply_matrix_on_vector (const Matrix* matrix_ptr,       const Vector* vector_ptr,        Vector* result_vector_ptr);
int multiply_matrix_on_matrix (const Matrix* first_matrix_ptr, const Matrix* second_matrix_ptr, Matrix* result_matrix_ptr);
//==============================================================================


#endif // __VECTOR_MATRIX_H_INCLUDED
