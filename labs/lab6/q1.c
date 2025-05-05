#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char bool;
#define true 1
#define false 0

typedef struct {
    char chr;
    int weight;
} huffman_node_t;
// 哈夫曼节点类型定义

#define TreeElemType huffman_node_t

typedef struct binary_tree_node {
    TreeElemType data;
    int weight;

    struct binary_tree_node* parent;

    struct binary_tree_node* left; // encode as 0
    struct binary_tree_node* right; // encode as 1
} binary_tree_node_t;
// 二叉树节点类型定义

typedef struct binary_tree {
    binary_tree_node_t* root;
} binary_tree_t;

binary_tree_t* create_binary_tree() {
    binary_tree_t* tree = (binary_tree_t*)malloc(sizeof(binary_tree_t));
    tree->root = NULL;
    return tree;
}
// 二叉树创建

binary_tree_node_t* create_binary_tree_node(TreeElemType data) {
    binary_tree_node_t* node = (binary_tree_node_t*)malloc(sizeof(binary_tree_node_t));
    node->data = data;
    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;
    node->weight = -1;
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

huffman_node_t* read_huffman_nodes(int* size) {
    int n;
    scanf("%d", &n);
    *size = n;

    huffman_node_t* nodes = (huffman_node_t*)malloc(sizeof(huffman_node_t) * n);
    int cnt = 0;
    while (cnt < n) {
        char c;
        scanf("%c", &c);
        if (c == ' ') {
            continue;
        }
        nodes[cnt].chr = c;
        cnt++;
    }
    for (int i = 0; i < n; i++) {
        scanf("%d", &nodes[i].weight);
    }
    return nodes;
}

void _find_two_min_weight(binary_tree_node_t** tree_nodes, int n, int* min1, int* min2) {
    int min_weight = 0x7fffffff;
    for (int i = 0; i < n; i++) {
        if (*(tree_nodes + i) == NULL) {
            continue;
        }
        if (tree_nodes[i]->weight < min_weight) {
            min_weight = tree_nodes[i]->weight;
            *min1 = i;
        }
    }
    min_weight = 0x7fffffff;
    for (int i = 0; i < n; i++) {
        if (*(tree_nodes + i) == NULL) {
            continue;
        }
        if (tree_nodes[i]->weight < min_weight && i != *min1) {
            min_weight = tree_nodes[i]->weight;
            *min2 = i;
        }
    }
}
// 寻找两个最小权重

binary_tree_node_t* _merge_two(binary_tree_node_t* node1, binary_tree_node_t* node2) {
    binary_tree_node_t* node = create_binary_tree_node((TreeElemType){0, node1->weight + node2->weight});
    node->left = node1;
    node->right = node2;

    node1->parent = node;
    node2->parent = node;

    node->weight = node1->weight + node2->weight;
    return node;
}
// 合并两个节点

binary_tree_t* from_huffman_nodes(huffman_node_t* huffman_nodes, int n) {
    binary_tree_t* tree = create_binary_tree();
    binary_tree_node_t* tree_nodes[n];

    for (int i = 0; i < n; i++) {
        tree_nodes[i] = create_binary_tree_node(huffman_nodes[i]);
        tree_nodes[i]->weight = huffman_nodes[i].weight;
    }

    int original = n;
    while (n > 1) {
        int min1, min2;
        _find_two_min_weight(&tree_nodes[0], original, &min1, &min2);
        tree_nodes[min1] = _merge_two(tree_nodes[min1], tree_nodes[min2]);
        tree_nodes[min2] = NULL;
        n -= 1;
    }

    for (int i = 0; i < original; i++) {
        if (tree_nodes[i] != NULL) {
            tree->root = tree_nodes[i];
            break;
        }
    }

    return tree;
}

bool encode_char(binary_tree_node_t* node, char chr, char* buf, int* len) {
    if (node == NULL) {
        return false;
    }

    if (node->data.chr == chr) {
        buf[(*len)++] = '\0';
        return true;
    }

    if (node->left != NULL) {
        buf[(*len)++] = '0';
        bool result = encode_char(node->left, chr, buf, len);
        (*len)--;

        if (result) {
            return true;
        }
    }

    if (node->right != NULL) {
        buf[(*len)++] = '1';
        bool result = encode_char(node->right, chr, buf, len);
        (*len)--;

        if (result) {
            return true;
        }
    }
    return false;
}

void decode_char(binary_tree_node_t* node, char* buf, int* len, char* chr) {
    if (node == NULL) {
        return;
    }

    if (node->left == NULL && node->right == NULL) {
        *chr = node->data.chr;
        return;
    }

    if (buf[*len] == '0') {
        decode_char(node->left, buf, len, chr);
    } else {
        decode_char(node->right, buf, len, chr);
    }
}

void readline(char* buf) {
    fgets(buf, 1024, stdin);
    buf[strlen(buf) - 1] = '\0';
}

int main() {
    int n;
    huffman_node_t* huffman_nodes = read_huffman_nodes(&n);
    binary_tree_t* tree = from_huffman_nodes(huffman_nodes, n);

    char* line = (char*)malloc(1024);
    scanf("%s", line);
    for (int i = 0; i < strlen(line); i++) {
        char chr = line[i];
        char buf[1024] = {0};
        int len = 0;
        encode_char(tree->root, chr, buf, &len);
        printf("%s", buf);
    }
    printf("\n");
    printf("%s\n", line);
    free(line);

    destroy_binary_tree(tree);
    free(huffman_nodes);
    return 0;
}