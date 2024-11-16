
#ifndef ARBRE_H
#define ARBRE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "load.h"



typedef struct TreeNode TreeNode;

 struct TreeNode {
    struct Table *table;
    struct TreeNode *left;
    struct TreeNode *right;
};



TreeNode* initialize_tree(struct Table *table);
TreeNode* create_node(struct Table *table);
TreeNode* insert(TreeNode* node, struct Table *table);
TreeNode* search(TreeNode* root, const char* tableName);

#endif
