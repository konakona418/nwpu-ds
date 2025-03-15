#include <stdio.h>
#include <stdlib.h>

typedef int bool;
#define true 1
#define false 0

#define T char

typedef struct node {
    T data;
    int cnt;
    struct node* prev;
    struct node* next;
} node_t;

typedef struct {
    int size;
    node_t* sentinel;
} list_t;

node_t* create_node(T val) {
    node_t* node = malloc(sizeof(node_t));
    node->prev = NULL;
    node->next = NULL;
    node->data = val;
    node->cnt = 0;
    return node;
}

list_t* create_list() {
    list_t* list = malloc(sizeof(list_t));
    node_t* sentinel = create_node((T) NULL);
    sentinel->prev = sentinel;
    sentinel->next = sentinel;
    list->sentinel = sentinel;
    list->size = 0;
    return list;
}

typedef void(*iter_fn)(node_t*, int);

void iter(list_t* list, iter_fn cb) {
    node_t* node = list->sentinel->next;
    int idx = 0;
    while (node != list->sentinel) {
        node_t* tmp = node->next;
        cb(node, idx);
        node = tmp;
        idx++;
    }
}

void destroy_node(node_t* node) {
    free(node);
}

void _destroy_node_callback(node_t* node, int idx) {
    destroy_node(node);
}

void destroy_list(list_t* list) {
    iter(list, _destroy_node_callback);
    destroy_node(list->sentinel);
    free(list);
}

T nth(list_t* list, int idx) {
    node_t* node = list->sentinel->next;
    for (int i = 0; i < idx; i++) {
        node = node->next;
    }
    node->cnt++;
    return node->data;
}

node_t* find_node(list_t* list, T val) {
    node_t* node = list->sentinel->next;
    while (node != list->sentinel) {
        if (node->data == val) {
            node->cnt++;
            return node;
        }
        node = node->next;
    }
    return NULL;
}

void remove_node(list_t* list, node_t* node) {
    node_t* it = list->sentinel->next;
    while (it != list->sentinel) {
        if (it->data == node->data) {
            node->prev->next = node->next;
            node->next->prev = node->prev;
            destroy_node(node);
            list->size--;
            break;
        }
        it = it->next;
    }
}

void push_back(list_t* list, T val) {
    node_t* node = create_node(val);
    node_t* last = list->sentinel->prev;

    last->next = node;
    node->prev = last;
    node->next = list->sentinel;
    list->sentinel->prev = node;

    list->size++;
}

T pop_back(list_t* list) {
    node_t* last = list->sentinel->prev;

    last->prev->next = list->sentinel;
    list->sentinel->prev = last->prev;

    T val = last->data;
    destroy_node(last);
    list->size--;
    return val;
}

void push_front(list_t* list, T val) {
    node_t* first = list->sentinel->next;
    node_t* node = create_node(val);

    list->sentinel->next = node;
    node->prev = list->sentinel;
    node->next = first;
    first->prev = node;

    list->size++;
}

T pop_front(list_t* list) {
    node_t* first = list->sentinel->next;
    list->sentinel->next = first->next;
    first->next->prev = list->sentinel;

    T val = first->data;
    destroy_node(first);
    list->size--;
    return val;
}

node_t* find_max_freq(list_t* list) {
    node_t* node = list->sentinel->next;
    node_t* max_node = node;

    while (node != list->sentinel) {
        if (node->cnt > max_node->cnt) {
            max_node = node;
        }
        node = node->next;
    }
    return max_node;
}

node_t* peek_front(list_t* list) {
    return list->sentinel->next;
}

node_t* peek_back(list_t* list) {
    return list->sentinel->prev;
}

int main() {
    list_t* list = create_list();
    int n, op;
    scanf("%d %d", &n, &op);
    for (int i = 0; i < n; i++) {
        T item;
        scanf("%c", &item);
        if (item < 'a' || item > 'z') {
            i--;
            continue;
        }
        push_back(list, item);
    }

    for (int i = 0; i < op; i++) {
        T item;
        scanf("%c", &item);
        if (item < 'a' || item > 'z') {
            i--;
            continue;
        }

        node_t* node = find_node(list, item);

        int cnt = node->cnt;

        remove_node(list, node);

        push_back(list, item);

        peek_back(list)->cnt = cnt;
    }

    while (list->size > 0) {
        node_t* max = find_max_freq(list);
        printf("%c ", max->data);
        remove_node(list, max);
    }
    printf("\n");

    destroy_list(list);
    return 0;
}