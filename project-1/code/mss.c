/**
 * @file mss.c
 * @brief This file contains function implementations for the problem.
 */

#include <stdio.h>
#include <stdlib.h>
#include "mss.h"

/**
 * @brief Create a Matrix object
 *
 * This function uses malloc to allocate memory for the matrix structure and the
 * matrix elements. So you need not to allocate memory for the matrix before
 * calling this function.
 *
 * Because there is pointer in the matrix structure, you need to use
 * FreeMatrix() function to free the memory allocated for the matrix.
 */
Matrix *CreateMatrix(const int rows, const int cols)
{
    Matrix *m = (Matrix *)malloc(sizeof(Matrix));
    m->rows = rows;
    m->cols = cols;
    m->data = (int *)malloc(sizeof(int) * rows * cols);
    // m->sum = 0;
    return m;
}

/**
 * @brief Copy a Matrix object
 *
 * This function creates a new matrix and copies the elements from the original
 * matrix to the new matrix.
 */
Matrix *CopyMatrix(Matrix *m)
{
    Matrix *copy = CreateMatrix(m->rows, m->cols);
    for (int i = 0; i < m->rows; i++)
    {
        for (int j = 0; j < m->cols; j++)
        {
            copy->data[i * copy->cols + j] = m->data[i * m->cols + j];
        }
    }
    // copy->sum = m->sum;
    return copy;
}

/**
 * @brief Read Matrix Elements from File
 *
 * This function reads matrix elements from the file following row-major order.
 *
 * Rows and cols should be read from the file before calling this function.
 * Numbers of rows and cols of the matrix should already exist in the matrix
 * structure. This function only reads the matrix elements.
 */
void ReadMatrix(Matrix *m, FILE *fp)
{
    if (m->rows <= 0 || m->cols <= 0)
    {
        printf("Error: rows or cols of the matrix is not set.\n");
        return;
    }

    for (int i = 0; i < m->rows; i++)
    {
        for (int j = 0; j < m->cols; j++)
        {
            int return_value = fscanf(fp, "%d", &m->data[i * m->cols + j]);
            if (return_value == EOF)
            {
                printf("Error: not enough elements in the file.\n");
                return;
            }
            else if (return_value == 0)
            {
                printf("Error: invalid element.\n");
                return;
            }
        }
    }
}

/**
 * @brief Print Matrix to File
 *
 * This function prints matrix elements to the file following row-major order.
 */
void PrintMatrix(Matrix *m, FILE *fp)
{
    for (int i = 0; i < m->rows; i++)
    {
        for (int j = 0; j < m->cols; j++)
        {
            fprintf(fp, "%-8d ", m->data[i * m->cols + j]);
        }
        fprintf(fp, "\n");
    }
}

/**
 * @brief Free the memory allocated for the matrix.
 *
 * This function frees the memory allocated for the matrix structure and the
 * matrix elements.
 *
 * There is pointer in the matrix structure, so you need to use this function to
 * free the memory allocated for the matrix.
 *
 * Matrix pointer should be set to NULL if not been initialized or was freed.
 *
 * This function will check if the pointer is NULL and prevent double free, so
 * you need not to check it before calling this function.
 */
void FreeMatrix(Matrix *m)
{
    // Always remember to check if the pointer is NULL.
    if (m != NULL)
    {
        // Double free will cause error.
        if (m->data != NULL)
        {
            free(m->data);
            m->data = NULL;
        }
        free(m);
        m = NULL;
    }
}

/**
 * @brief The naive version of the maximum submatrix sum algorithm.
 *
 * This algorithm simply enumerates all possible submatrices and finds the one
 * with the maximum sum. The variable i, j, k, l means the submatrix is from row
 * i to row k and from column j to column l. For each combination of i, j, k, l,
 * it traverses all elements in the submatrix and calculates the sum. If the sum
 * is larger than the maximum sum, it updates the maximum sum and the maximum
 * submatrix. After traversing all possible submatrices, it returns the maximum
 * submatrix.
 */
Matrix *MaxSubmatrixN6(Matrix *m)
{
    int max_sum = 0;
    int max_i = 0;
    int max_j = 0;
    int max_k = 0;
    int max_l = 0;
    // Enumerate all possible submatrices.
    for (int i = 0; i < m->rows; i++)
    {
        for (int j = 0; j < m->cols; j++)
        {
            for (int k = i; k < m->rows; k++)
            {
                for (int l = j; l < m->cols; l++)
                {
                    // Calculate the sum of the submatrix.
                    int sum = 0;
                    for (int x = i; x <= k; x++)
                    {
                        for (int y = j; y <= l; y++)
                        {
                            sum += m->data[x * m->cols + y];
                        }
                    }
                    // Update the maximum sum and the maximum submatrix.
                    if (sum > max_sum)
                    {
                        max_sum = sum;
                        max_i = i;
                        max_j = j;
                        max_k = k;
                        max_l = l;
                    }
                }
            }
        }
    }
    // Create the maximum submatrix.
    Matrix *result = CreateMatrix(max_k - max_i + 1, max_l - max_j + 1);
    for (int i = max_i; i <= max_k; i++)
        for (int j = max_j; j <= max_l; j++)
            result->data[(i - max_i) * result->cols + (j - max_j)] = m->data[i * m->cols + j];
    return result;
}

/**
 * @brief This function implements the N4 version of the maximum submatrix sum
 * algorithm.
 *
 * Compared to the N6 version, this algorithm reduces the time complexity from
 * O(n^6) to O(n^4). It sums each row of the submatrix and stores the sum in an
 * array. Then it uses Kadane's algorithm to find the maximum subarray sum of
 * the array.
 */
Matrix *MaxSubmatrixN4(Matrix *m)
{
    int max_sum = 0;
    int max_left = 0;
    int max_right = 0;
    int max_top = 0;
    int max_bottom = 0;
    int *row_sums = (int *)malloc(sizeof(int) * m->rows);
    // The left and right bound of the submatrix.
    // Time complexity: O(n^2).
    for (int left = 0; left < m->cols; left++)
    {
        // When the left bound changes, reset the row sums.
        for (int i = 0; i < m->rows; i++)
            row_sums[i] = 0;
        for (int right = left; right < m->cols; right++)
        {
            // Append the new column to the sum.
            // Time complexity: O(n).
            for (int i = 0; i < m->rows; i++)
                row_sums[i] += m->data[i * m->cols + right];
            // Naive method to find the maximum sum of the array.
            // Time complexity: O(n^2)
            int sum = 0;
            for (int i = 0; i < m->rows; i++)
            {
                sum = 0;
                for (int j = i; j < m->rows; j++)
                {
                    sum += row_sums[j];
                    if (sum > max_sum)
                    {
                        max_sum = sum;
                        max_left = left;
                        max_right = right;
                        max_top = i;
                        max_bottom = j;
                    }
                }
            }
        }
    }
    free(row_sums);
    // Create the maximum submatrix.
    Matrix *result = CreateMatrix(max_bottom - max_top + 1, max_right - max_left + 1);
    for (int i = max_top; i <= max_bottom; i++)
        for (int j = max_left; j <= max_right; j++)
            result->data[(i - max_top) * result->cols + (j - max_left)] = m->data[i * m->cols + j];
    return result;
}

/**
 * @brief This function implements my version of the maximum submatrix sum
 * algorithm.
 *
 * It substitutes the N4 version of finding the maximum subarray sum with
 * Kadane's algorithm. This reduces the time complexity from O(n^4) to O(n^3).
 */
Matrix *MaxSubmatrix(Matrix *m)
{
    int max_sum = 0;
    int max_left = 0;
    int max_right = 0;
    int max_top = 0;
    int max_bottom = 0;
    int *row_sums = (int *)malloc(sizeof(int) * m->rows);
    // The left and right bound of the submatrix.
    // Time complexity: O(n^2).
    for (int left = 0; left < m->cols; left++)
    {
        // When the left bound changes, reset the row sums.
        for (int i = 0; i < m->rows; i++)
            row_sums[i] = 0;
        for (int right = left; right < m->cols; right++)
        {
            // Append the new column to the sum.
            // Time complexity: O(n).
            for (int i = 0; i < m->rows; i++)
                row_sums[i] += m->data[i * m->cols + right];
            // Kadane's algorithm to find the maximum sum of the array.
            // Time complexity: O(n)
            int sum = 0;
            int top = 0;
            for(int i = 0; i < m->rows; i++)
            {
                sum += row_sums[i];
                if(sum < 0)
                {
                    sum = 0;
                    top = i + 1;
                }
                else if(sum > max_sum)
                {
                    max_sum = sum;
                    max_left = left;
                    max_right = right;
                    max_top = top;
                    max_bottom = i;
                }
            }
        }
    }
    free(row_sums);
    // Create the maximum submatrix.
    Matrix *result = CreateMatrix(max_bottom - max_top + 1, max_right - max_left + 1);
    for (int i = max_top; i <= max_bottom; i++)
        for (int j = max_left; j <= max_right; j++)
            result->data[(i - max_top) * result->cols + (j - max_left)] = m->data[i * m->cols + j];
    return result;
}
