#include <stdio.h>

#include "matrix.h"

#define EXPECTED_ARGC 4

void perform_benchmark(int n, int num_iterations, int tile_size);

int main(int argc, char* argv[])
{
    int n, num_iterations, tile_size;

    if (argc != EXPECTED_ARGC)
    {
        printf("Usage: mmperf <n> <num_iterations> <tile_size>\n");
        exit(1);
    }

    n = atoi(argv[1]);
    num_iterations = atoi(argv[2]);
    tile_size = atoi(argv[3]);

    printf("n = %i, tile size = %i\n", n, tile_size);

    perform_benchmark(n, num_iterations, tile_size);

    return 0;
}

void perform_benchmark(int n, int num_iterations, int tile_size)
{
    struct square_matrix a, b, result, result_aware;
    float temp;

    // Create and initialize matrices with dummy values.
    square_matrix_create(n, &a);
    square_matrix_create(n, &b);
    square_matrix_create(n, &result);
    square_matrix_create(n, &result_aware);
    
    temp = 1.0f;
    for (int i = 0; i != a.n; ++i)
    {
        for (int j = 0; j != a.n; ++j)
        {
            a.entries[i][j] = temp;
            temp += 1.0f;
        }
    }

    temp = 1.0f;
    for (int i = 0; i != b.n; ++i)
    {
        for (int j = 0; j != b.n; ++j)
        {
            b.entries[i][j] = temp;
            temp += 0.8f;
        }
    }

    // Benchmark naive implementation.
    for (int i = 0; i != num_iterations; ++i)
    {
        zeros(&result);
        naive_multiply(&a, &b, &result);
    }

    // Benchmark tiled implementation.
    for (int i = 0; i != num_iterations; ++i)
    {
        zeros(&result_aware);
        tiled_multiply(tile_size, &a, &b, &result_aware);
    }

    // Display result.
    printf("Done, %i iterations for n = %i\n", num_iterations, n);
    // Sanity check to verify approaches produce equal results.
    printf("Results match? %s\n", are_matrices_equal(&result, &result_aware) ? "yes" : "no");

    square_matrix_delete(&a);
    square_matrix_delete(&b);
    square_matrix_delete(&result);
    square_matrix_delete(&result_aware);
}
