#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define T int

typedef int bool;
#define true 1
#define false 0

typedef struct {
    int size;
    int capacity;

    T* data;
} vector_t;

vector_t* create_vector() {
    vector_t* vec = malloc(sizeof(vector_t));
    vec->size = 0;
    vec->capacity = 4;
    T* data = malloc(sizeof(T) * vec->capacity);
    vec->data = data;

    return vec;
}

void destroy_vector(vector_t* vec) {
    free(vec->data);
    free(vec);
}

#define at(vector, offset) (*(vector->data + offset))

void _reallocate(vector_t* vec) {
    T* new_data = malloc(sizeof(T) * vec->capacity * 2);
    memcpy(new_data, vec->data, sizeof(T) * vec->capacity);
    free(vec->data);
    vec->data = new_data;
    vec->capacity *= 2;
}

void push_back(vector_t* vec, T val) {
    if (vec->size >= vec->capacity) {
        _reallocate(vec);
    }
    vec->data[vec->size++] = val;
}

T pop_back(vector_t* vec) {
    if (vec->size == 0) {
        return *vec->data;
    }
    return vec->data[--vec->size];
}

void remove_item(vector_t* vec, T val) {
    int idx = -1;
    for (int i = 0; i < vec->size; i++) {
        if (vec->data[i] == val) {
            idx = i;
            break;
        }
    }
    if (idx == -1) {
        return;
    }
    memcpy(vec->data + idx, vec->data + idx + 1, sizeof(T) * (vec->size - idx - 1));
    vec->size--;
}

void print_vec(vector_t* vec) {
    for (int i = 0; i < vec->size; i++) {
        printf("%d ", at(vec, i));
    }
    printf("\n");
}

void insert_ordered(vector_t* vec, T val) {
    int idx = vec->size;
    for (int i = 0; i < vec->size; i++) {
        if (at(vec, i) > val) {
            idx = i;
            break;
        }
    }
    if (vec->size >= vec->capacity) {
        _reallocate(vec);
    }
    if (idx == vec->size) {
        push_back(vec, val);
        return;
    }
    memcpy(vec->data + idx + 1, vec->data + idx, sizeof(T) * (vec->size - idx));
    vec->data[idx] = val;
    vec->size++;
}

int main() {
    vector_t* vec = create_vector();
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        int x;
        scanf("%d", &x);
        push_back(vec, x);
    }
    int ins;
    scanf("%d", &ins);
    insert_ordered(vec, ins);
    print_vec(vec);
    destroy_vector(vec);
    return 0;
}