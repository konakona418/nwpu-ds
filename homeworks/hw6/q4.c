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

typedef struct {
    char* left;
    char* right;
} _string_pair_t;

_string_pair_t* _split_string(char* str, char splitter) {
    _string_pair_t* pair = (_string_pair_t*)malloc(sizeof(_string_pair_t));
    int offset = 0;
    char* it = str;
    while (*it != '\0') {
        if (*it == splitter) {
            break;
        }
        it++;
        offset++;
    }

    // ABCD E FGHI
    // 0123 4 5678

    char* left = offset > 0 ? (char*) malloc((offset + 1) * sizeof(char)) : NULL;
    if (left != NULL) {
        memcpy(left, str, offset * sizeof(char));
        left[offset] = '\0';
    }
    char* right = offset < strlen(str) - 1 ? (char*) malloc((strlen(str) - offset) * sizeof(char)) : NULL;
    if (right != NULL) {
        memcpy(right,  str + offset + 1, (strlen(str) - offset - 1) * sizeof(char));
        right[strlen(str) - offset - 1] = '\0';
    }
    pair->left = left;
    pair->right = right;

    return pair;
}

void _destroy_string_pair(_string_pair_t* pair) {
    if (pair == NULL) {
        return;
    }

    free(pair->left);
    free(pair->right);
    free(pair);
}

binary_tree_node_t* _from_strings_impl(char** str_pre, char** str_in) {
    char* pre_it = *str_pre;
    char* in_it = *str_in;

    binary_tree_node_t* node = create_binary_tree_node(*pre_it);

    _string_pair_t* pair = _split_string(in_it, *pre_it);
    pre_it++;

    if (pair->left != NULL) {
        char* pre_it_left = (char*) malloc((strlen(pair->left) + 1) * sizeof(char));
        pre_it_left[strlen(pair->left)] = '\0';

        memcpy(pre_it_left, pre_it, strlen(pair->left) * sizeof(char));

        node->left = _from_strings_impl(&pre_it_left, &pair->left);

        free(pre_it_left);
    }
    if (pair->right != NULL) {
        char* pre_it_right = (char*) malloc((strlen(pair->right) + 1) * sizeof(char));
        pre_it_right[strlen(pair->right)] = '\0';

        memcpy(pre_it_right, pre_it + (pair->left ? strlen(pair->left) : 0), strlen(pair->right) * sizeof(char));

        node->right = _from_strings_impl(&pre_it_right, &pair->right);

        free(pre_it_right);
    }
    _destroy_string_pair(pair);

    return node;
}

binary_tree_t* from_strings(char* str_pre, char* str_in) {
    binary_tree_t* tree = create_binary_tree();
    char** it_pre = &str_pre;
    char** it_in = &str_in;
    tree->root = _from_strings_impl(it_pre, it_in);
    return tree;
}

void _print_binary_tree_impl(binary_tree_node_t* node) {
    if (node == NULL) {
        return;
    }

    printf("%c", node->data);
    if (node->left == NULL && node->right == NULL) {
        return;
    }
    printf("(");
    if (node->left != NULL) {
        _print_binary_tree_impl(node->left);
    } else {
        printf("#");
    }
    printf(",");
    if (node->right != NULL) {
        _print_binary_tree_impl(node->right);
    } else {
        printf("#");
    }
    printf(")");
}

void print_binary_tree(binary_tree_t* tree) {
    if (tree == NULL) {
        return;
    }

    _print_binary_tree_impl(tree->root);
}

void _print_binary_tree_back_impl(binary_tree_node_t* node) {
    if (node == NULL) {
        return;
    }

    if (node->left != NULL) {
        _print_binary_tree_back_impl(node->left);
    }
    if (node->right != NULL) {
        _print_binary_tree_back_impl(node->right);
    }
    printf("%c", node->data);
}

void print_binary_tree_back(binary_tree_t* tree) {
    if (tree == NULL) {
        return;
    }

    _print_binary_tree_back_impl(tree->root);
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
    char str_pre[1024];
    char str_in[1024];
    scanf("%s", str_pre);
    scanf("%s", str_in);

    binary_tree_t* tree = from_strings(str_pre, str_in);
    print_binary_tree_back(tree);
    destroy_binary_tree(tree);
    return 0;
}