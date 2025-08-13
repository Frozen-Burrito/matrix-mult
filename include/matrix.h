#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>
#include <stdbool.h>

typedef float* matrix_t;

void square_matrix_create(matrix_t *const out_matrix, int n);
void square_matrix_delete(matrix_t *const matrix);

void zeros(matrix_t a, int n);
bool are_matrices_equal(const matrix_t a, const matrix_t b, int n);

void naive_multiply(const matrix_t a, const matrix_t b, matrix_t out_c, int n);
void tiled_multiply(const matrix_t a, const matrix_t b, matrix_t out_c, int n, int tile_size);

#endif /* MATRIX_H */
