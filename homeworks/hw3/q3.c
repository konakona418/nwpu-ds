#include <stdio.h>
#include <stdlib.h>

#define T int

typedef int bool;
#define true 1
#define false 0

typedef struct {
    T* data;
    int size;
    int start_ptr;
    int end_ptr;
} queue_t;

queue_t* create_queue(int size) {
    queue_t* queue = malloc(sizeof(queue_t));
    queue->data = (T*)malloc(sizeof(T) * size);
    queue->size = size;
    queue->start_ptr = 0;
    queue->end_ptr = 0;
    return queue;
}

void destroy_queue(queue_t* queue) {
    free(queue->data);
    free(queue);
}

bool push(queue_t* queue, T value) {
    bool is_full = (queue->size - 1 == queue->end_ptr);
    if (queue->end_ptr == queue->size) {
        queue->end_ptr = 0;
    }
    queue->data[queue->end_ptr++] = value;
    return is_full;
}

T pop(queue_t* queue) {
    if (queue->start_ptr == queue->size) {
        queue->start_ptr = 0;
    }
    T value = queue->data[queue->start_ptr++];
    return value;
}

void print_queue(queue_t* queue) {
    if (queue->start_ptr <= queue->end_ptr - 1) {
        for (int i = queue->start_ptr; i < queue->end_ptr - 1; i++) {
            printf("%d ", queue->data[i]);
        }
    } else {
        for (int i = queue->start_ptr; i < queue->size; i++) {
            printf("%d ", queue->data[i]);
        }
        for (int i = 0; i < queue->end_ptr - 1; i++) {
            printf("%d ", queue->data[i]);
        }
    }
    printf("%d", queue->data[queue->end_ptr - 1]);
}

int main() {
    int n;
    scanf("%d", &n);
    queue_t* queue = create_queue(n);
    fflush(stdin);
    bool full = false;

    char chr;
    do {
        int val;
        scanf("%d", &val);
        full |= push(queue, val);
    } while((chr=getchar()) != '\n');
    int x;
    scanf("%d", &x);
    while (pop(queue) != x) {
    }
    if (full) {
        printf("yes\n");
    } else {
        printf("no\n");
    }
    print_queue(queue);
    printf("\n");
    printf("%d", pop(queue));
    destroy_queue(queue);
}