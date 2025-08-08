#include "matrix.h"

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

void square_matrix_create(int n, struct square_matrix* const out_matrix)
{
    out_matrix->n = n;
    out_matrix->entries = (float **) malloc(sizeof(float*) * n);

    for (int i = 0; i != n; ++i)
    {
        out_matrix->entries[i] = (float *) malloc(sizeof(float) * n);
    }
}

void square_matrix_delete(struct square_matrix* a)
{
    for (int i = 0; i != a->n; ++i)
    {
        free(a->entries[i]);
    }

    free(a->entries);
    a->entries = NULL;
}

void zeros(struct square_matrix* const a)
{
    for (int i = 0; i != a->n; ++i)
    {
        for (int j = 0; j != a->n; ++j)
        {
            a->entries[i][j] = 0.0f;
        }
    }
}

bool are_matrices_equal(const struct square_matrix* const a, const struct square_matrix* const b)
{
    bool are_equal = true;

    for (int i = 0; i != a->n; ++i)
    {
        for (int j = 0; j != b->n; ++j)
        {
            if (a->entries[i][j] != b->entries[i][j])
            {
                are_equal = false;
                break;
            }
        }
    }

    return are_equal;
}

void naive_multiply(const struct square_matrix* const a, const struct square_matrix* const b, struct square_matrix* const out_c)
{
    for (int i = 0; i != a->n; ++i)
    {
        for (int k = 0; k != a->n; ++k)
        {
            for (int j = 0; j != a->n; ++j)
            {
                out_c->entries[i][j] += a->entries[i][k] * b->entries[k][j];
            }
        }
    }
}

void tiled_multiply(int tile_size, const struct square_matrix* const a, const struct square_matrix* const b, struct square_matrix* const out_c)
{
    int tile_row_end, tile_col_end, tile_inner_end;

    for (int tile_i = 0; tile_i < a->n; tile_i += tile_size)
    {
        tile_row_end = MIN((tile_i + tile_size), a->n);
        for (int tile_j = 0; tile_j < a->n; tile_j += tile_size)
        {
            tile_col_end = MIN((tile_j + tile_size), a->n);
            for (int tile_k = 0; tile_k < a->n; tile_k += tile_size)
            {
                tile_inner_end = MIN((tile_k + tile_size), a->n);
                for (int i = tile_i; i < tile_row_end; ++i)
                {
                    for (int k = tile_k; k < tile_inner_end; ++k)
                    {
                        for (int j = tile_j; j < tile_col_end; ++j)
                        {
                            out_c->entries[i][j] += a->entries[i][k] * b->entries[k][j];
                        }
                    }
                }
            }
        }
    }
}
