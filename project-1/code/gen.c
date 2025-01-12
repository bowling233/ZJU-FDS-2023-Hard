/**
 * @file gen_data.c
 * @brief Random matrix data generator.
 *
 * This program generates random square matrix data and stores them in files.
 *
 * @section usage Usage
 *
 * ./gen_data <N> <num_of_files>
 *
 * N: The rank of the matrix.
 *
 * num_of_files: The number of files to be generated.
 *
 * The generated data will be stored in the file "data/N_index.txt". Index
 * ranges from 0 to num_of_files - 1.
 */
#include <stdio.h>
#include <stdlib.h>
#include "mss.h"

int main(int argc, char *argv[])
{
    // Check arguments
    if (argc != 3)
    {
        printf("Usage: ./gen_data <N> <num_of_files>\n");
        return 0;
    }
    // Get arguments
    int N = atoi(argv[1]);
    int num_of_files = atoi(argv[2]);
    // Generate data
    char filename[30];
    Matrix *m = CreateMatrix(N, N);
    for(int i = 0; i < num_of_files; i++)
    {
        sprintf(filename, "data/%d_%d.txt", N, i);
        FILE *fp = fopen(filename, "w");
        // Check file
        if(fp == NULL)
        {
            printf("Error: cannot open file %s.\n", filename);
            return 0;
        }
        // Write data
        fprintf(fp, "%d %d\n", N, N);
        for (int i = 0; i < N * N; i++)
        {
            m->data[i] = rand() % 200 - 100;
        }
        PrintMatrix(m, fp);
        fclose(fp);
    }
    FreeMatrix(m);

    return 0;
}
