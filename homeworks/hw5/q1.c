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

matrix_t* mat_add(matrix_t* mat1, matrix_t* mat2) {
    matrix_t* sum_mat = create_matrix(mat1->row, mat1->col);
    for (int i = 0; i < mat1->data->size; i++) {
        VecElemType* triplet = at_ref(mat1->data, i);
        mat_set(sum_mat, triplet->row, triplet->col, triplet->val);
    }
    for (int i = 0; i < mat2->data->size; i++) {
        VecElemType* triplet = at_ref(mat2->data, i);
        MatrixElemType* ptr = mat_at(sum_mat, triplet->row, triplet->col);
        if (ptr == NULL) {
            mat_set(sum_mat, triplet->row, triplet->col, triplet->val);
        } else {
            *ptr += triplet->val;
        }
    }
    return sum_mat;
}

void sort_mat(matrix_t* mat) {
    for (int i = 0; i < mat->data->size; i++) {
        for (int j = i + 1; j < mat->data->size; j++) {
            VecElemType* triplet1 = at_ref(mat->data, i);
            VecElemType* triplet2 = at_ref(mat->data, j);
            if (triplet1->row > triplet2->row) {
                VecElemType temp = *triplet1;
                *triplet1 = *triplet2;
                *triplet2 = temp;
            }
        }
    }
    for (int i = 0; i < mat->data->size; i++) {
        for (int j = i + 1; j < mat->data->size; j++) {
            VecElemType* triplet1 = at_ref(mat->data, i);
            VecElemType* triplet2 = at_ref(mat->data, j);
            if (triplet1->row == triplet2->row && triplet1->col > triplet2->col) {
                VecElemType temp = *triplet1;
                *triplet1 = *triplet2;
                *triplet2 = temp;
            }
        }
    }
}

void print_mat(matrix_t* mat) {
    for (int i = 0; i < mat->data->size; i++) {
        VecElemType* triplet = at_ref(mat->data, i);
        if (triplet->val == 0) {
            continue;
        }
        printf("%d %d %d\n", triplet->row + 1, triplet->col + 1, triplet->val);
    }
}

void read_mat(matrix_t* mat, int elem) {
    for (int i = 0; i < elem; i++) {
        int row, col, val;
        scanf("%d %d %d", &row, &col, &val);
        if (row == 0 || col == 0) {
            continue;
        }
        mat_set(mat, row - 1, col - 1, val);
    }
}

int main() {
    int x, y;
    scanf("%d %d", &x, &y);
    matrix_t* mat1 = create_matrix(0, 0);
    read_mat(mat1, x);
    matrix_t* mat2 = create_matrix(0, 0);
    read_mat(mat2, y);

    matrix_t* sum = mat_add(mat1, mat2);
    sort_mat(sum);
    print_mat(sum);

    destroy_matrix(mat1);
    destroy_matrix(mat2);
    destroy_matrix(sum);
}