#include <stdio.h>
#include <stdlib.h>

typedef int bool;
typedef struct node {
    int data;
    struct node* next;
} node_t;

typedef struct {
    int size;
    node_t* sentinel;
} list_t;

node_t* create_node(int val) {
    node_t* node = (node_t*) malloc(sizeof(node_t));
    node->data = val;
    node->next = NULL;
    return node;
}

void destroy_node(node_t* node) {
    // printf("list dtor invoked: %d\n", node->data);
    free(node);
}

list_t* create_list() {
    list_t* list = (list_t*) malloc(sizeof(list_t));
    list->size = 0;
    node_t* sentinel = create_node(-1);
    list->sentinel = sentinel;
    sentinel->next = sentinel;
    return list;
}

typedef void(*iter_cb)(node_t*);
void iter(list_t* list, iter_cb cb) {
    node_t* node = list->sentinel->next;
    while (node != list->sentinel) {
        node_t* tmp = node->next;
        cb(node);
        node = tmp;
    }
}

void destroy_list(list_t* list) {
    iter(list, destroy_node);
    destroy_node(list->sentinel);
    free(list);
}

node_t* find_last(list_t* list) {
    node_t* node = list->sentinel;
    while (node->next != list->sentinel) {
        node = node->next;
    }
    return node;
}

node_t* find_nth(list_t* list, int n) {
    node_t* node = list->sentinel;
    for (int i = 0; i < n + 1; i++) {
        node = node->next;
    }
    return node;
}

bool contains(list_t* list, int val) {
    bool flag = 0;
    node_t* node = list->sentinel->next;
    while (node != list->sentinel) {
        node_t* tmp = node->next;
        flag = node->data == val;
        if (flag) {
            break;
        }
        node = tmp;
    }
    return flag;
}

bool both_contains(list_t* l1, list_t* l2, int val) {
    bool flag = 0;
    flag = contains(l1, val) && contains(l2, val);

    return flag;
}

void push_back(list_t* list, int val) {
    node_t* new_node = create_node(val);
    node_t* last = find_last(list);
    last->next = new_node;
    new_node->next = list->sentinel;
    list->size++;
}

void print_node(node_t* node) {
    printf("%d ", node->data);
}

void print_list(list_t* list) {
    iter(list, print_node);
    printf("\n");
}

void print_list_new(list_t* list) {
    node_t* node = list->sentinel->next;
    while (node != list->sentinel) {
        node_t* tmp = node->next;
        if (node->next != list->sentinel) {
            printf("%d ", node->data);
        } else {
            printf("%d", node->data);
        }
        node = tmp;
    }
    printf("\n");
}

list_t* revert_list(list_t* list) {
    list_t* reverted = create_list();
    for (int i = list->size - 1; i >= 0; i--) {
        push_back(reverted, find_nth(list, i)->data);
    }
    return reverted;
}

void print_revert_array(int n, int arr[n]) {
    for (int i = n - 1; i >= 1; i--) {
        printf("%d ", arr[i]);
    }
    printf("%d", arr[0]);
    printf("\n");
}

list_t* read_list(int size) {
    list_t* list = create_list();
    for (int i = 0; i < size; i++) {
        int x;
        scanf("%d", &x);
        push_back(list, x);
    }
    return list;
}

int main() {
    int s1, s2, s3;
    scanf("%d %d %d", &s1, &s2, &s3);
    list_t* l1 = read_list(s1);
    list_t* l2 = read_list(s2);
    list_t* l3 = read_list(s3);

    node_t* prev = l1->sentinel;
    node_t* node = l1->sentinel->next;
    while (node != l1->sentinel) {
        if (both_contains(l2, l3, node->data)) {
            prev->next = node->next;
            destroy_node(node);
            node = prev->next;
        } else {
            prev = node;
            node = node->next;
        }
    }

    print_list_new(l1);
    destroy_list(l1);
    destroy_list(l2);
    destroy_list(l3);

    return 0;
}