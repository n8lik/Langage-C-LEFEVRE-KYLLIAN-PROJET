
#ifndef ARBRE_H
#define ARBRE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "load.h"

typedef enum {TABLE_NODE, COLUMN_NODE, VALUE_NODE} NodeType;

typedef struct TreeNode TreeNode;

struct TreeNode {
    NodeType type;
    union {
        struct Table *table;
        struct Column *column;
        char *value;
    } data;
    struct TreeNode *left;
    struct TreeNode *right;
    struct TreeNode *firstChild;  
};

TreeNode* create_node(NodeType type, void* data);
TreeNode* insert(TreeNode* node, NodeType type, void* data);

TreeNode* search(TreeNode* root, const char* tableName);



void print_tree(TreeNode* root, int level);
#endif
