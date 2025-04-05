#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MatrixElemType int

typedef struct {
    int row;
    int col;
    MatrixElemType val;
} triplet_t;

#define VecElemType triplet_t

typedef int bool;
#define true 1
#define false 0

typedef struct {
    int size;
    int capacity;

    VecElemType* data;
} vector_t;

vector_t* create_vector() {
    vector_t* vec = malloc(sizeof(vector_t));
    vec->size = 0;
    vec->capacity = 4;
    VecElemType* data = malloc(sizeof(VecElemType) * vec->capacity);
    vec->data = data;

    return vec;
}

void destroy_vector(vector_t* vec) {
    free(vec->data);
    free(vec);
}

#define at(vector, offset) (*(vector->data + offset))
#define at_ref(vector, offset) (vector->data + offset)

void _reallocate(vector_t* vec) {
    VecElemType* new_data = malloc(sizeof(VecElemType) * vec->capacity * 2);
    memcpy(new_data, vec->data, sizeof(VecElemType) * vec->capacity);
    free(vec->data);
    vec->data = new_data;
    vec->capacity *= 2;
}

void push_back(vector_t* vec, VecElemType val) {
    if (vec->size >= vec->capacity) {
        _reallocate(vec);
    }
    vec->data[vec->size++] = val;
}

VecElemType pop_back(vector_t* vec) {
    if (vec->size == 0) {
        return *vec->data;
    }
    return vec->data[--vec->size];
}

typedef struct {
    int row;
    int col;

    vector_t* data;
} matrix_t;

matrix_t* create_matrix(int m, int n) {
    matrix_t* mat = malloc(sizeof(matrix_t));
    mat->row = m;
    mat->col = n;
    mat->data = create_vector();

    return mat;
}

void destroy_matrix(matrix_t* mat) {
    destroy_vector(mat->data);
    free(mat);
}

MatrixElemType* mat_at(matrix_t* mat, int row, int col) {
    for (int i = 0; i < mat->data->size; i++) {
        VecElemType* triplet = at_ref(mat->data, i);
        if (triplet->row == row && triplet->col == col) {
            return &triplet->val;
        }
    }
    return NULL;
}

void mat_set(matrix_t* mat, int row, int col, MatrixElemType val) {
    MatrixElemType* ptr = mat_at(mat, row, col);
    if (ptr == NULL) {
        VecElemType triplet = {row, col, val};
        push_back(mat->data, triplet);
    } else {
        *ptr = val;
    }
}

matrix_t* mat_transpose(matrix_t* mat) {
    matrix_t* transposed_mat = create_matrix(mat->col, mat->row);
    for (int i = 0; i < mat->data->size; i++) {
        VecElemType* triplet = at_ref(mat->data, i);
        VecElemType transposed_triplet = {triplet->col, triplet->row, triplet->val};
        push_back(transposed_mat->data, transposed_triplet);
    }
    return transposed_mat;
}

void print_mat(matrix_t* mat) {
    for (int i = 0; i < mat->data->size; i++) {
        VecElemType* triplet = at_ref(mat->data, i);
        printf("%d %d %d\n", triplet->row + 1, triplet->col + 1, triplet->val);
    }
}

int main() {
    int m, n;
    scanf("%d %d", &m, &n);
    matrix_t* mat = create_matrix(m, n);

    while (true) {
        int row, col, val;
        scanf("%d %d %d", &row, &col, &val);
        if (row == 0 && col == 0 && val == 0) {
            break;
        }
        mat_set(mat, row - 1, col - 1, val);
    }
    matrix_t* transposed_mat = mat_transpose(mat);
    print_mat(transposed_mat);

    destroy_matrix(mat);
    destroy_matrix(transposed_mat);
}