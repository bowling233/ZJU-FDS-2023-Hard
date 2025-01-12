/**
 * @file main.c
 * @brief Driver program for the maximum submatrix sum problem.
 *
 * This program runs test for the project and append the result to the report
 * file "report.csv".
 *
 * @section usage Usage
 *
 * ./mss <datafile> <algorithm> [iteration]
 *
 * - datafile: The name of the data file.
 *
 * - algorithm: The algorithm to be tested. 1 means the N6 version, 2 means the
 *   N4 version, 3 means my version.
 *
 * - iteration: The number of iterations to run the algorithm. If not specified,
 *   the program will run the algorithm at least once until the total time is
 *   more than 5 second.
 *
 * This program will print the result matrix to the standard output.
 *
 * @mainpage Maximum Submatrix Sum Project
 *
 * @section intro_sec Introduction
 *
 * The goal of this project is to find the maximum submatrix sum of a given
 * matrix.
 *
 * @section datafile_sec Data File Format
 *
 * The data file stores the matrix data. The first line is the number of rows
 * and cols of the matrix. The following lines are the elements of the matrix.
 * For example:
 *
 * @code
 * 3 3 
 * 1 2 3 
 * 4 5 6 
 * 7 8 9
 * @endcode
 *
 * @section report_sec Report File Format
 *
 * This program will append the result to the report file "report.csv". The
 * format is:
 *
 * <table>
 * <tr><td>datasize</td><td>algorithm</td><td>iteration</td><td>ticks</td><td>total_time</td><td>duration</td></tr>
 * <tr><td>100</td><td>1</td><td>10</td><td>123456</td><td>0.123456</td><td>0.012345</td></tr>
 * </table>
 */

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "mss.h"

int main(int argc, char *argv[])
{
    // Check the number of arguments.
    if (argc != 4 && argc != 3)
    {
        printf("Usage: ./mss <datafile> <algorithm> [iteration]\n");
        return 0;
    }

    // Read the matrix from the file.
    char *filename = argv[1];
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Error: cannot open file %s.\n", filename);
        return 0;
    }
    int n, m;
    // Never ignore the return value of fscanf.
    int return_value = fscanf(fp, "%d %d", &n, &m);
    if (return_value != 2)
    {
        printf("Error: invalid data file.\n");
        return 0;
    }
    Matrix *mat = CreateMatrix(n, m);
    ReadMatrix(mat, fp);
    fclose(fp);

    // Check if there is no positive element in the matrix.
    for(int i = 0; i < n * m; i++)
    {
        if(mat->data[i] > 0)
            break;
        if(i == n * m - 1)
        {
            printf("Error: no positive element in the matrix.\n");
            return 0;
        }
    }
    
    // Run the algorithm and calculate the time.
    int algorithm = atoi(argv[2]);
    int iteration = 0;
    if (argc == 4)
        iteration = atoi(argv[3]);
    int ticks = 0;
    double total_time = 0;
    double duration = 0;
    Matrix *result = NULL;
    clock_t start, end;
    int i = 0;
    // Run the algorithm at least once
    do
    {
        start = clock();
        // Use different algorithm according to the argument.
        switch (algorithm)
        {
        case 1:
            result = MaxSubmatrixN6(mat);
            break;
        case 2:
            result = MaxSubmatrixN4(mat);
            break;
        case 3:
            result = MaxSubmatrix(mat);
            break;
        default:
            printf("Error: invalid algorithm.\n");
            return 0;
        }
        end = clock();
        // Calculate the time.
        ticks += end - start;
        total_time += (double)(end - start) / CLOCKS_PER_SEC;
        duration += (double)(end - start) / CLOCKS_PER_SEC;
        i++;
        // End the loop if the total time is more than 5 second. Or if the
        // iteration is specified, end the loop if the iteration is reached.
    }while((argc == 3 && total_time < 5) || i < iteration);
    FreeMatrix(mat);
    duration /= i;

    // Print the result matrix to the standard output.
    printf("datafile: %s\n", argv[1]);
    printf("algorithm: %d\n", algorithm);
    printf("MaxSubmatrix: \n");
    PrintMatrix(result, stdout);
    FreeMatrix(result);

    // Test if the report file already exists. Or else create a new one and
    // write the header.
    fp = fopen("report.csv", "r");
    if (fp == NULL)
    {
        fp = fopen("report.csv", "w");
        fprintf(fp, "datasize,algorithm,iteration,ticks,total_time,duration\n");
    }
    fclose(fp);

    // Append the result to the report file.
    fp = fopen("report.csv", "a");
    fprintf(fp, "%d,%d,%d,%d,%lf,%lf\n", n, algorithm, iteration, ticks, total_time, duration);
    fclose(fp);

    return 0;
}
