#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char bool;
#define true 1
#define false 0

#define TreeElemType char

typedef struct binary_tree_node {
    TreeElemType data;

    struct binary_tree_node* left;
    struct binary_tree_node* right;
} binary_tree_node_t;

typedef struct binary_tree {
    binary_tree_node_t* root;
} binary_tree_t;

binary_tree_t* create_binary_tree() {
    binary_tree_t* tree = (binary_tree_t*)malloc(sizeof(binary_tree_t));
    tree->root = NULL;
    return tree;
}

binary_tree_node_t* create_binary_tree_node(TreeElemType data) {
    binary_tree_node_t* node = (binary_tree_node_t*)malloc(sizeof(binary_tree_node_t));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void destroy_binary_tree_node(binary_tree_node_t* node) {
    if (node == NULL) {
        return;
    }

    destroy_binary_tree_node(node->left);
    destroy_binary_tree_node(node->right);
    free(node);
}

void destroy_binary_tree(binary_tree_t* tree) {
    if (tree == NULL) {
        return;
    }

    destroy_binary_tree_node(tree->root);
    free(tree);
}

binary_tree_node_t* from_string(char** str) {
    binary_tree_node_t* node = create_binary_tree_node('\0');
    char* it = *str;

    if (*it == '#') {
        it++;
        destroy_binary_tree_node(node);
        *str = it;
        return NULL;
    }

    node->data = *it;
    it++;

    binary_tree_node_t* left = from_string(&it);
    node->left = left;
    binary_tree_node_t* right = from_string(&it);
    node->right = right;
    *str = it;
    return node;
}

binary_tree_t* from_string_to_binary_tree(char* str) {
    binary_tree_t* tree = create_binary_tree();
    char** it = &str;
    tree->root = from_string(it);
    return tree;
}

void print_binary_tree_node(binary_tree_node_t* node) {
    if (node == NULL) {
        return;
    }

    printf("%c", node->data);
    if (node->left == NULL && node->right == NULL) {
        return;
    }
    printf("(");
    if (node->left != NULL) {
        print_binary_tree_node(node->left);
    } else {
        printf("#");
    }
    printf(",");
    if (node->right != NULL) {
        print_binary_tree_node(node->right);
    } else {
        printf("#");
    }
    printf(")");
}

void print_binary_tree(binary_tree_t* tree) {
    if (tree == NULL) {
        return;
    }

    print_binary_tree_node(tree->root);
}

void readline(char* buf) {
    fgets(buf, 1024, stdin);
    buf[strlen(buf) - 1] = '\0';
}

int _count_leaves_impl(binary_tree_node_t* node) {
    if (node == NULL) {
        return 0;
    }

    if (node->left == NULL && node->right == NULL) {
        return 1;
    }

    return _count_leaves_impl(node->left) + _count_leaves_impl(node->right);
}

int count_leaves(binary_tree_t* tree) {
    return _count_leaves_impl(tree->root);
}

int main() {
    char str[1024];
    scanf("%s", str);
    binary_tree_t* tree = from_string_to_binary_tree(str);
    printf("%d\n", count_leaves(tree));
    destroy_binary_tree(tree);
    return 0;
}