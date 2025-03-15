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

T peek(stack_t* stack) {
    return stack->data[stack->size - 1];
}

void read_line(char* str) {
    fscanf(stdin, "%s", str);
}

#define is_operator_lvl1(_op) (_op == '+' || _op == '-')
#define is_operator_lvl2(_op) (_op == '*' || _op == '/')
#define is_left_bracket(_chr) (_chr == '(')
#define is_right_bracket(_chr) (_chr == ')')

#define is_operand(_chr) (_chr >= 'a' && _chr <= 'z')
#define is_operator(_chr) (!is_operand(_chr))

bool is_operator_prior(char op1, char op2) {
    if (is_operator_lvl1(op1) && is_operator_lvl1(op2)) {
        return false;
    }
    if (is_operator_lvl2(op1) && is_operator_lvl2(op2)) {
        return false;
    }
    if (is_operator_lvl2(op1) && is_operator_lvl1(op2)) {
        return true;
    }
    if (is_operator_lvl1(op1) && is_operator_lvl2(op2)) {
        return false;
    }
    return false;
}

int main() {
    stack_t* operators = create_stack();

    char str[128] = {0};
    read_line(str);

    char output[128] = {0};
    int output_idx = 0;

    for (int i = 0; str[i] != '\0'; ++i) {
        if (is_operand(str[i])) {
            output[output_idx++] = str[i];
            continue;
        }
        if (is_left_bracket(str[i])) {
            push_back(operators, str[i]);
            continue;
        }
        if (is_right_bracket(str[i])) {
            while (true) {
                char op = pop_back(operators);
                if (is_left_bracket(op)) {
                    break;
                }
                output[output_idx++] = op;
            }
            continue;
        }
        if (is_operator(str[i])) {
            char top = peek(operators);
            if (operators->size <= 0 || !is_operator_prior(top, str[i])) {
                push_back(operators, str[i]);
                continue;
            }
            while (operators->size > 0 && is_operator_prior(top, str[i])) {
                output[output_idx++] = pop_back(operators);
                top = peek(operators);
            }
            push_back(operators, str[i]);
        }
    }
    while (operators->size > 0) {
        output[output_idx++] = pop_back(operators);
    }

    for (int i = 0; i < output_idx; ++i) {
        printf("%c", output[i]);
    }
    destroy_stack(operators);
    return 0;
}