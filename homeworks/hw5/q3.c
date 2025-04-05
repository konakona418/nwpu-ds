#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char bool;
#define true 1
#define false 0

#define TableElemType char

struct table;
struct table_elem;

typedef enum {
    TABLE_ELEM_DATA,
    TABLE_ELEM_SUB_TABLE
} table_elem_type_t;

typedef struct table_elem {
    struct table_elem* next;

    union {
        TableElemType data;
        struct table* sub_table;
    };
    table_elem_type_t type;
} table_elem_t;

#define VecElemType table_elem_t

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
    vector_t* elements;
    int size;
} table_t;

table_t* create_table() {
    table_t* table = (table_t*)malloc(sizeof(table_t));
    table->elements = create_vector();
    table->size = 0;
    return table;
}

void destroy_table(table_t* table) {
    destroy_vector(table->elements);
    free(table);
}

table_t* parse(char* str) {
    table_t* table = create_table();
    while (*str) {
        if (*str == '(') {
            table_t* sub_table = parse(str + 1);
            push_back(table->elements, (VecElemType){.type = TABLE_ELEM_SUB_TABLE, .sub_table = sub_table});
        }
        else if (*str == ')') {
            return table;
        }
        else if (*str != ',') {
            push_back(table->elements, (VecElemType){.type = TABLE_ELEM_DATA, .data = *str});
        }
        str++;
    }
}

int depth_impl(table_t* table) {
    int max_depth = 0;
    for (int i = 0; i < table->elements->size; i++) {
        if (at(table->elements, i).type == TABLE_ELEM_SUB_TABLE) {
            int d = depth_impl(at(table->elements, i).sub_table);
            if (d > max_depth) {
                max_depth = d;
            }
        }
    }
    return max_depth + 1;
}

int depth(table_t* table) {
    return depth_impl(table) - 1;
}

int main() {
    char* str = malloc(128);
    scanf("%s", str);
    table_t* table = parse(str);
    printf("%d\n", depth(table));
    printf("%d\n", depth(table));
    destroy_table(table);
    return 0;
}