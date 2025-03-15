#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define T int

typedef int bool;
#define true 1
#define false 0

typedef struct {
    T* data;
    int size;
    int end_ptr;
} queue_t;

queue_t* create_queue(int size) {
    queue_t* queue = malloc(sizeof(queue_t));
    queue->data = (T *) malloc(sizeof(T) * size);
    queue->size = size;
    queue->end_ptr = 0;
    return queue;
}

void destroy_queue(queue_t* queue) {
    free(queue->data);
    free(queue);
}

void push_back(queue_t* queue, T value) {
    if (queue->end_ptr == queue->size) {
        memcpy(queue->data, queue->data + 1, sizeof(T) * (queue->size - 1));
        queue->data[queue->size - 1] = value;
        return;
    }
    queue->data[queue->end_ptr++] = value;
}

void push_front(queue_t* queue, T value) {
    memcpy(queue->data + 1, queue->data, sizeof(T) * (queue->size - 1));
    queue->data[0] = value;
    if (queue->end_ptr <= queue->size)
        queue->end_ptr++;
}

T pop_front(queue_t* queue) {
    T value = queue->data[0];
    memcpy(queue->data, queue->data + 1, sizeof(T) * (queue->end_ptr - 1));
    queue->end_ptr--;
    return value;
}

T pop_back(queue_t* queue) {
    if (queue->end_ptr <= 0) {
        return queue->data[0];
    }
    T value = queue->data[queue->end_ptr - 1];
    queue->end_ptr--;
    return value;
}

void print_queue(queue_t* queue) {
    for (int i = 0; i < queue->end_ptr - 1; i++) {
        printf("%d ", queue->data[i]);
    }
    printf("%d\n", queue->data[queue->end_ptr - 1]);
}

int main() {
    int n, k;
    scanf("%d %d", &n, &k);
    queue_t* queue = create_queue(n);

    for (int i = 0; i < n; i++) {
        int x;
        scanf("%d", &x);
        push_back(queue, x);
    }

    for (int i = 0; i < k; i++) {
        int x = pop_back(queue);
        push_front(queue, x);
    }

    print_queue(queue);
    destroy_queue(queue);
}
