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

#define is_normal_char(c) (((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z') || ((c) >= '0' && (c) <= '9') || ((c) == '#'))

binary_tree_node_t* from_string(char* str, int* offset) {
    binary_tree_node_t* node = create_binary_tree_node('\0');
    char* it = str;

    *offset = 0;
    char* initial = it;

    while (*it != '\0') {
        if (is_normal_char(*it)) {
            node->data = *it;
            if (*(it + 1) != '(') {
                *offset = it - initial + 1;
                return node;
            }
        } else if (*it == '(') {
            int o = 0;
            binary_tree_node_t* child = from_string(it + 1, &o);
            node->left = child;
            it += o;
        } else if (*it == ')') {
            it++;
            break;
        } else if (*it == ',') {
            int o = 0;
            node->right = from_string(it + 1, &o);
            it += o;
        }
        it++;
    }
    *offset = it - initial;
    return node;
}

binary_tree_t* from_string_to_binary_tree(char* str) {
    binary_tree_t* tree = create_binary_tree();
    int offset = 0;
    tree->root = from_string(str, &offset);
    return tree;
}

void print_binary_tree_node(binary_tree_node_t* node) {
    if (node == NULL) {
        return;
    }

    printf("%c", node->data);
    if (node->left != NULL) {
        print_binary_tree_node(node->left);
    }
    if (node->right != NULL) {
        print_binary_tree_node(node->right);
    }
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

int main() {
    char str[1024];
    readline(str);
    binary_tree_t* tree = from_string_to_binary_tree(str);
    print_binary_tree(tree);
    destroy_binary_tree(tree);
    return 0;
}