#include "matrix.h"

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

void square_matrix_create(matrix_t *const out_matrix, int n)
{
    *out_matrix = (float *) malloc(sizeof(float) * n * n);
}

void square_matrix_delete(matrix_t *const a)
{
    free(*a);
    *a = NULL;
}

void zeros(matrix_t a, int n)
{
    for (int i = 0; i != n; ++i)
    {
        for (int j = 0; j != n; ++j)
        {
            a[i * n + j] = 0.0f;
        }
    }
}

bool are_matrices_equal(const matrix_t a, const matrix_t b, int n)
{
    bool are_equal = true;

    for (int i = 0; i != n; ++i)
    {
        for (int j = 0; j != n; ++j)
        {
            if (a[i * n + j] != b[i * n + j])
            {
                are_equal = false;
                break;
            }
        }
    }

    return are_equal;
}

void naive_multiply(const matrix_t a, const matrix_t b, matrix_t  out_c, int n)
{
    for (int i = 0; i != n; ++i)
    {
        for (int j = 0; j != n; ++j)
        {
            for (int k = 0; k != n; ++k)
            {
                out_c[i * n + j] += a[i * n + k] * b[k * n + j];
            }
        }
    }
}

void tiled_multiply(const matrix_t a, const matrix_t  b, matrix_t out_c, int n, int tile_size)
{
    int tile_row_end, tile_col_end, tile_inner_end;

    for (int tile_i = 0; tile_i < n; tile_i += tile_size)
    {
        tile_row_end = MIN((tile_i + tile_size), n);
        for (int tile_j = 0; tile_j < n; tile_j += tile_size)
        {
            tile_col_end = MIN((tile_j + tile_size), n);
            for (int tile_k = 0; tile_k < n; tile_k += tile_size)
            {
                tile_inner_end = MIN((tile_k + tile_size), n);
                for (int i = tile_i; i < tile_row_end; ++i)
                {
                    for (int k = tile_k; k < tile_inner_end; ++k)
                    {
                        for (int j = tile_j; j < tile_col_end; ++j)
                        {
                           out_c[i * n + j] += a[i * n + k] * b[k * n + j];
                        }
                    }
                }
            }
        }
    }
}
