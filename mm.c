#include <stdio.h>
#include <stdlib.h>

#define EXPECTED_ARGC 2
#define NUM_ITERATIONS  1000

struct square_matrix {
    float** entries;
    int n;
};

void square_matrix_create(int n, struct square_matrix* const out_matrix);
void square_matrix_delete(struct square_matrix* a);

void naive_multiply(const struct square_matrix* const a, const struct square_matrix* const b, struct square_matrix* const out_c);
void aware_multiply(const struct square_matrix* const a, const struct square_matrix* const b, struct square_matrix* const out_c);

int main(int argc, char* argv[])
{
    struct square_matrix a;
    struct square_matrix b;
    struct square_matrix result;
    int i;
    int j;
    int n;
    float temp;

    if (argc != EXPECTED_ARGC)
    {
        printf("Usage: mmperf <n>\n");
        exit(1);
    }

    n = atoi(argv[1]);

    // Initialize memory for matrices.
    square_matrix_create(n, &a);
    square_matrix_create(n, &b);
    square_matrix_create(n, &result);
    
    // Assign dummy values to A's entries. 
    temp = 1.0f;
    for (i = 0; i != a.n; ++i)
    {
        for (j = 0; j != a.n; ++j)
        {
            a.entries[i][j] = temp;
            temp += 1.0f;
        }
    }

    // Assign dummy values to B's entries.
    temp = 1.0f;
    for (i = 0; i != b.n; ++i)
    {
        for (j = 0; j != b.n; ++j)
        {
            b.entries[i][j] = temp;
            temp += 0.8f;
        }
    }

    for (i = 0; i != NUM_ITERATIONS; ++i)
    {
        naive_multiply(&a, &b, &result);
    }

    // Display result.
    printf("Done, %i iterations for n = %i\n", NUM_ITERATIONS, n);

    square_matrix_delete(&a);
    square_matrix_delete(&b);
    square_matrix_delete(&result);

    return 0;
}

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

void naive_multiply(const struct square_matrix* const a, const struct square_matrix* const b, struct square_matrix* const out_c)
{
    int i;
    int j;
    int k;

    for (i = 0; i != a->n; ++i)
    {
        for (j = 0; j != a->n; ++j)
        {
            out_c->entries[i][j] = 0.0f;

            for (k = 0; k != a->n; ++k)
            {
                out_c->entries[i][j] += a->entries[i][k] * b->entries[k][j];
            }
        }
    }
}

void aware_multiply(const struct square_matrix* const a, const struct square_matrix* const b, struct square_matrix* const out_c)
{

}
