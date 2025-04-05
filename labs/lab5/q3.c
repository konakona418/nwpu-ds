#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MatrixElemType int

typedef struct triplet_node {
    int row;
    int col;
    MatrixElemType val;

    struct triplet_node *row_next; // right
    struct triplet_node *col_next; // down
} triplet_node_t;

triplet_node_t* create_triplet_node(int row, int col, MatrixElemType val) {
    triplet_node_t* node = (triplet_node_t*)malloc(sizeof(triplet_node_t));
    node->row = row;
    node->col = col;
    node->val = val;
    node->row_next = NULL;
    node->col_next = NULL;
    return node;
}

void destroy_triplet_node(triplet_node_t* node) {
    free(node);
}

void push_down(triplet_node_t* head, triplet_node_t* node) {
    triplet_node_t* cur = head;
    while (cur->col_next != NULL) {
        cur = cur->col_next;
    }
    cur->col_next = node;
}

void push_right(triplet_node_t* head, triplet_node_t* node) {
    triplet_node_t* cur = head;
    while (cur->row_next != NULL) {
        cur = cur->row_next;
    }
    cur->row_next = node;
}

typedef struct {
    int row;
    int col;

    triplet_node_t* head;
} matrix_t;

matrix_t* create_matrix(int row, int col) {
    matrix_t* m = malloc(sizeof(matrix_t));
    m->row = row;
    m->col = col;

    m->head = create_triplet_node(0, 0, 0);

    for (int i = 0; i < row; i++) {
        push_down(m->head, create_triplet_node(i + 1, 0, 0));
    }

    for (int i = 0; i < col; i++) {
        push_right(m->head, create_triplet_node(0, i + 1, 0));
    }

    return m;
}

void destroy_matrix(matrix_t* m) {
    triplet_node_t* row_cur = m->head;
    while (row_cur != NULL) {
        triplet_node_t* col_cur = row_cur->col_next;
        while (col_cur != NULL) {
            triplet_node_t* tmp = col_cur;
            col_cur = col_cur->row_next;
            destroy_triplet_node(tmp);
        }
        row_cur = row_cur->row_next;
    }
    while (m->head != NULL) {
        triplet_node_t* tmp = m->head;
        m->head = m->head->row_next;
        destroy_triplet_node(tmp);
    }
    free(m);
}

triplet_node_t* _find_col_in_row(triplet_node_t* head, int row, int col) {
    triplet_node_t* cur = head;
    for (int i = 0; i < row; i++) {
        cur = cur->col_next;
    }
    triplet_node_t* col_cur = cur->col_next;
    while (col_cur->row_next != NULL) {
        if (col_cur->row_next->col > col) {
            return col_cur;
        }
        col_cur = col_cur->row_next;
    }
    if (col_cur->col == col) {
        return col_cur;
    }
    return NULL;
}

void mat_set(matrix_t* m, int row, int col, MatrixElemType val) {
    if (row < 1 || row > m->row || col < 1 || col > m->col) {
        return;
    }
    triplet_node_t* col_cur = _find_col_in_row(m->head, row, col);
    if (col_cur != NULL) {
        if (col_cur->col == col) {
            col_cur->val = val;
            return;
        }
        if (col_cur->row_next->col > col) {
            triplet_node_t* new_node = create_triplet_node(row, col, val);
            new_node->row_next = col_cur->row_next;
            col_cur->row_next = new_node;
            return;
        }
    }

    triplet_node_t* cur = m->head;
    for (int i = 0; i < row; i++) {
        cur = cur->col_next;
    }
    push_right(cur, create_triplet_node(row, col, val));
}

MatrixElemType* mat_at(matrix_t* m, int row, int col) {
    if (row < 1 || row > m->row || col < 1 || col > m->col) {
        return NULL;
    }
    triplet_node_t* cur = m->head;
    while (cur != NULL) {
        if (cur->row == row) {
            break;
        }
        cur = cur->col_next;
    }
    while (cur != NULL) {
        if (cur->col == col) {
            return &cur->val;
        }
        cur = cur->row_next;
    }
    return NULL;
}

void print_mat(matrix_t* m) {
    triplet_node_t* cur = m->head->col_next;
    while (cur != NULL) {
        triplet_node_t* col_cur = cur->row_next;
        while (col_cur != NULL) {
            printf("%d %d %d\n", col_cur->row , col_cur->col, col_cur->val);
            col_cur = col_cur->row_next;
        }
        cur = cur->col_next;
    }
}

void read_mat(matrix_t* m, int elem) {
    for (int i = 0; i < elem; i++) {
        int row, col, val;
        scanf("%d %d %d", &row, &col, &val);
        mat_set(m, row, col, val);
    }
}

matrix_t* mat_sum(matrix_t* mat1, matrix_t* mat2) {
    matrix_t* sum = create_matrix(mat1->row, mat1->col);
    for (int i = 1; i <= mat1->row; i++) {
        for (int j = 1; j <= mat1->col; j++) {
            MatrixElemType* val1 = mat_at(mat1, i, j);
            MatrixElemType* val2 = mat_at(mat2, i, j);
            if (val1 != NULL && val2 != NULL) {
                mat_set(sum, i, j, *val1 + *val2);
                continue;
            }
            if (val1 != NULL) {
                mat_set(sum, i, j, *val1);
                continue;
            }
            if (val2 != NULL) {
                mat_set(sum, i, j, *val2);
            }
        }
    }
    return sum;
}

int main() {
    int m, n, x, y;
    scanf("%d %d %d %d", &m, &n, &x, &y);
    matrix_t* mat1 = create_matrix(m, n);
    read_mat(mat1, x);
    matrix_t* mat2 = create_matrix(m, n);
    read_mat(mat2, y);

    matrix_t* sum = mat_sum(mat1, mat2);
    print_mat(sum);

    destroy_matrix(mat1);
    destroy_matrix(mat2);
    destroy_matrix(sum);
}