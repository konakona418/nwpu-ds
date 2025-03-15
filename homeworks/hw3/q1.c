#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define T char

typedef int bool;
#define true 1
#define false 0

typedef struct {
    int size;
    int capacity;

    T* data;
} stack_t;

stack_t* create_stack() {
    stack_t* stack = malloc(sizeof(stack_t));
    stack->size = 0;
    stack->capacity = 4;
    stack->data = malloc(sizeof(T) * stack->capacity);

    return stack;
}

void destroy_stack(stack_t* stack) {
    free(stack->data);
    free(stack);
}

void _reallocate(stack_t* stack) {
    T* new_data = malloc(sizeof(T) * stack-> capacity * 2);
    memcpy(new_data, stack->data, sizeof(T) * stack->capacity);
    stack->capacity *= 2;
    free(stack->data);
    stack->data = new_data;
}

void push_back(stack_t* stack, T data) {
    if (stack->size >= stack->capacity) {
        _reallocate(stack);
    }
    stack->data[stack->size++] = data;
}

T pop_back(stack_t* stack) {
    if (stack->size <= 0) {
        return stack->data[0];
    }

    return stack->data[--stack->size];
}

#define is_left_brake(_c) (_c == '(' || _c == '[' || _c == '{')
#define is_right_brake(_c) (_c == ')' || _c == ']' || _c == '}')
#define is_brake_match(_a, _b) ((_a == '(' && _b == ')') || (_a == '[' && _b == ']') || (_a == '{' && _b == '}'))

int main() {
    char buffer[128];

    stack_t* stack = create_stack();
    for (int i = 0; i < 128; i++) {
        char c = getchar();
        if (c == '\n') {
            buffer[i] = '\0';
            break;
        }
        buffer[i] = c;
    }

    for (int i = 0; i< strlen(buffer); i++) {
        if (is_left_brake(buffer[i])) {
            push_back(stack, buffer[i]);
        } else if (is_right_brake(buffer[i])) {
            char last = pop_back(stack);
            if (!is_brake_match(last, buffer[i])) {
                printf("no\n");
                destroy_stack(stack);
                return 0;
            }
        }
    }

    if (stack->size > 0) {
        printf("no\n");
    } else {
        printf("yes\n");
    }

    destroy_stack(stack);
    return 0;
}