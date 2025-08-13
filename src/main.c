#include <stdio.h>
#include <string.h>

#include "matrix.h"

#define EXPECTED_ARGC 4

void benchmark_naive(int n, int num_iterations);
void benchmark_tiled(int n, int tile_size, int num_iterations);

int main(int argc, char* argv[])
{
    int n, num_iterations, tile_size;

    if (argc < EXPECTED_ARGC)
    {
        printf("Usage: matrix_mult <both|naive|tiled> <n> <num_iterations> [tile_size]\n");
        exit(1);
    }

    n = atoi(argv[2]);
    num_iterations = atoi(argv[3]);

    printf("Matrix multiply with n = %i, %i iterations\n", n, num_iterations);

    if (0 == strcmp(argv[1], "both") || 0 == strcmp(argv[1], "naive"))
    {
        printf("Naive multiply\n");
        benchmark_naive(n, num_iterations);
    }

    if (0 == strcmp(argv[1], "both") || 0 == strcmp(argv[1], "tiled"))
    {
        tile_size = atoi(argv[4]);
        printf("Tiled multiply, tile size = %i\n", tile_size);
        benchmark_tiled(n, tile_size, num_iterations);
    }

    printf("Done\n");

    return 0;
}

void benchmark_naive(int n, int num_iterations)
{
    struct square_matrix a, b, result;
    float temp;

    // Create and initialize matrices with placeholder values.
    square_matrix_create(n, &a);
    square_matrix_create(n, &b);
    square_matrix_create(n, &result);

    temp = 1.0f;
    for (int i = 0; i != a.n; ++i)
    {
        for (int j = 0; j != a.n; ++j)
        {
            a.entries[i][j] = temp - 0.1f;
            b.entries[i][j] = temp + 0.3f;
            temp += 1.0f;
        }
    }

    for (int i = 0; i != num_iterations; ++i)
    {
        zeros(&result);
        naive_multiply(&a, &b, &result);
    }

    square_matrix_delete(&a);
    square_matrix_delete(&b);
    square_matrix_delete(&result);
}

void benchmark_tiled(int n, int tile_size, int num_iterations)
{
    struct square_matrix a, b, result;
    float temp;

    // Create and initialize matrices with placeholder values.
    square_matrix_create(n, &a);
    square_matrix_create(n, &b);
    square_matrix_create(n, &result);

    temp = 1.0f;
    for (int i = 0; i != a.n; ++i)
    {
        for (int j = 0; j != a.n; ++j)
        {
            a.entries[i][j] = temp - 0.1f;
            b.entries[i][j] = temp + 0.3f;
            temp += 1.0f;
        }
    }

    for (int i = 0; i != num_iterations; ++i)
    {
        zeros(&result);
        tiled_multiply(tile_size, &a, &b, &result);
    }

    square_matrix_delete(&a);
    square_matrix_delete(&b);
    square_matrix_delete(&result);
}
