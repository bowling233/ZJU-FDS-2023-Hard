/**
 * @file mss.h
 * @brief Matrix interface for the maximum submatrix sum problem.
 * 
 * This file contains the definition of the matrix structure and interface for the maximum submatrix sum problem.
 */
#ifndef _MSS_H_
#define _MSS_H_

/**
 * @brief Matrix structure
 *
 * This structure is simply a wrapper of the matrix elements and its number of
 * rows and columns.
 *
 * Though the problem in PTA only requires the input matrix to be a square
 * matrix, I still use a general matrix structure to make the program more
 * flexible. The result matrix is also a general matrix.
 */
struct Matrix
{
    int rows;
    int cols;
    int *data;
};
typedef struct Matrix Matrix;

/**
 * @brief Create a Matrix object
 * 
 * @param rows Rows of the matrix.
 * @param cols Columns of the matrix.
 * @return Matrix* Pointer to the matrix.
 */
Matrix* CreateMatrix(const int rows, const int cols);

/**
 * @brief Copy a Matrix object
 *
 * @param m Pointer to the matrix.
 * @return Matrix* Pointer to the new matrix.
 */
Matrix* CopyMatrix(Matrix *m);

/**
 * @brief Read Matrix Elements from File
 * 
 * @param m Pointer to the matrix.
 * @param fp Pointer to the file to be read.
 */
void ReadMatrix(Matrix *m, FILE *fp);

/**
 * @brief Print Matrix to File
 * 
 * @param m Pointer to the matrix.
 * @param fp Pointer to the file to be written.
 */
void PrintMatrix(Matrix *m, FILE *fp);

/**
 * @brief Free the memory allocated for the matrix.
 *
 * @param m Pointer to the matrix.
 */
void FreeMatrix(Matrix *m);

/** @defgroup mss Maximum Submatrix Sum
 * @brief Maximum Submatrix Sum
 *
 * This module contains the functions for the maximum submatrix sum problem.
 * Their APIs are unified to make it easy to test them.
 *
 * All the functions in this module take a matrix as input and return another
 * matrix as output, even if the result is the same as the input (to prevent
 * double free problem). The input matrix is not modified. There is no
 * constraint on the input matrix. The output matrix is also a general matrix.
 *
 * @param m Pointer to the matrix.
 * @return Matrix* Pointer to the result matrix.
 *
 * @{
 */ 
Matrix* MaxSubmatrixN6(Matrix *m);
Matrix* MaxSubmatrixN4(Matrix *m);
Matrix* MaxSubmatrix(Matrix *m);
/** @} */ // end of mss

#endif

