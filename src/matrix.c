#include "matrix.h"

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

void square_matrix_create(int n, struct square_matrix* const out_matrix)
{
    int i;

    out_matrix->n = n;
    out_matrix->entries = (float **) malloc(sizeof(float*) * n);

    for (i = 0; i != n; ++i)
    {
        out_matrix->entries[i] = (float *) malloc(sizeof(float) * n);
    }
}

void square_matrix_delete(struct square_matrix* a)
{
    int i;

    for (i = 0; i != a->n; ++i)
    {
        free(a->entries[i]);
    }

    free(a->entries);
    a->entries = NULL;
}

void zeros(struct square_matrix* const a)
{
    int i, j;
    for (i = 0; i != a->n; ++i)
    {
        for (j = 0; j != a->n; ++j)
        {
            a->entries[i][j] = 0.0f;
        }
    }
}

bool are_matrices_equal(const struct square_matrix* const a, const struct square_matrix* const b)
{
    int i, j;
    bool are_equal = true;

    for (i = 0; i != a->n; ++i)
    {
        for (j = 0; j != b->n; ++j)
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
    int i, j, k;

    for (i = 0; i != a->n; ++i)
    {
        for (j = 0; j != a->n; ++j)
        {
            for (k = 0; k != a->n; ++k)
            {
                out_c->entries[i][j] += a->entries[i][k] * b->entries[k][j];
            }
        }
    }
}

void tiled_multiply(int tile_size, const struct square_matrix* const a, const struct square_matrix* const b, struct square_matrix* const out_c)
{
    int i, j, k, tile_i, tile_j, tile_k;

    for (tile_i = 0; tile_i < a->n; tile_i += tile_size)
    {
        for (tile_j = 0; tile_j < a->n; tile_j += tile_size)
        {
            for (tile_k = 0; tile_k < a->n; tile_k += tile_size)
            {
                for (i = tile_i; i < MIN((tile_i + tile_size), a->n); ++i)
                {
                    for (j = tile_j; j < MIN((tile_j + tile_size), a->n); ++j)
                    {
                        for (k = tile_k; k < MIN((tile_k + tile_size), a->n); ++k)
                        {
                            out_c->entries[i][j] += a->entries[i][k] * b->entries[k][j];
                        }
                    }
                }
            }
        }
    }
}
