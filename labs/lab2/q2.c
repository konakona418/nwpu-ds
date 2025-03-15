#include <stdio.h>
#include <stdlib.h>

typedef int bool;
#define true 1
#define false 0

typedef unsigned int u32;

#define T u32

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

void _print_node_callback(node_t* node, int idx) {
    printf("%d ", node->data);
}

void print_list(list_t* list) {
    printf("[");
    iter(list, _print_node_callback);
    printf("]\n");
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

node_t* nth_node(list_t* list, int idx) {
    node_t* node = list->sentinel->next;
    for (int i = 0; i < idx; i++) {
        node = node->next;
    }
    node->cnt++;
    return node;
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

void force_align(list_t* list, int align) {
    while (list->size < align) {
        push_back(list, 0);
    }
    while (list->size > align) {
        pop_back(list);
    }
}

void list_mul(list_t* list, int n) {
    u32 overflow = 0;
    node_t* node = list->sentinel->prev; // back traverse.
    while (node != list->sentinel) {
        u32 val = node->data;
        val = val * n + overflow;
        node->data = val % 10;
        overflow = val / 10;

        node = node->prev;
    }
    if (overflow > 0) {
        push_front(list, overflow);
    }
}

void list_div(list_t* list, int n, int len_lim) {
    force_align(list, len_lim);
    u32 overflow = 0;
    node_t* node = list->sentinel->next; // front traverse.
    while (node != list->sentinel) {
        u32 val = node->data;
        val = val + overflow * 10;
        node->data = val / n;
        overflow = val % n;
        node = node->next;
    }
    while (overflow > 0) {
        u32 val = overflow * 10;
        push_back(list, val / n);
        overflow = val % n;

        if (list->size > len_lim) {
            return;
        }
    }
}

void list_add(list_t* list, list_t* item) {
    int align = list->size > item->size ? list->size : item->size;
    force_align(list, align);
    force_align(item, align);
    node_t* list_zeroth = peek_back(list); // in case that the two lists have different length.
    node_t* item_zeroth = peek_back(item);

    u32 overflow = 0;
    while (list_zeroth != list->sentinel && item_zeroth != item->sentinel) {
        u32 val = list_zeroth->data + item_zeroth->data + overflow;
        list_zeroth->data = val % 10;
        overflow = val / 10;
        list_zeroth = list_zeroth->prev; // still back traverse here.
        item_zeroth = item_zeroth->prev;
    }
}

void from_int(list_t* list, int n) {
    if (n == 0) {
        push_front(list, 0);
        return;
    }
    while (n > 0) {
        push_front(list, n % 10);
        n /= 10;
    }
}

#include <math.h>
int find_precision(int precision) {
    double k = 1;
    double s = 0;
    while (s < precision + 1) {
        s += log10((2 * k + 1) / k);
        k += 1;
    }
    return k * 2;
}

void print_list_as_digits(list_t* list) {
    node_t* node = list->sentinel->next;
    printf("%d.", node->data);
    node = node->next;
    while (node != list->sentinel) {
        printf("%d", node->data);
        node = node->next;
    }
    printf("\n");
}

int main() {
    list_t* sum = create_list(); // pi

    int n = 1;
    list_t* item = create_list(); // R(n)

    int precision;
    scanf("%d", &precision);

    from_int(item, 2); // initialize the two lists respectively.
    from_int(sum, 2);

    force_align(item, 550);
    force_align(sum, 550);


    for (int i = 0; i < find_precision(precision); i++) {
        list_mul(item, n);
        list_div(item, 2 * n + 1, 550);
        list_add(sum, item);

        n ++;
    }

    force_align(sum, precision + 1);
    print_list_as_digits(sum);
    destroy_list(sum);
    destroy_list(item);
    return 0;
}