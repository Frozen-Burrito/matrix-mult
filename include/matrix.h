#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>
#include <stdbool.h>

struct square_matrix {
    float** entries;
    int n;
};

void square_matrix_create(int n, struct square_matrix* const out_matrix);
void square_matrix_delete(struct square_matrix* a);

void zeros(struct square_matrix* const a);
bool are_matrices_equal(const struct square_matrix* const a, const struct square_matrix* const b);

void naive_multiply(const struct square_matrix* const a, const struct square_matrix* const b, struct square_matrix* const out_c);
void tiled_multiply(int tile_size, const struct square_matrix* const a, const struct square_matrix* const b, struct square_matrix* const out_c);

#endif /* MATRIX_H */
