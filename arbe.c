#include "arbre.h"


struct TreeNode *root = NULL;



TreeNode* create_node(NodeType type, void* data)
{
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    newNode->type = type;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->firstChild = NULL;

    switch(type) {
        case TABLE_NODE:
            newNode->data.table = (struct Table*)data;
            break;
        case COLUMN_NODE:
            newNode->data.column = (struct Column*)data;
            break;
        case VALUE_NODE:
            newNode->data.value = (char*)data;
            break;
    }

    return newNode;
}
TreeNode* insert(TreeNode* node, NodeType type, void* data)
{
    if (node == NULL) {
        return create_node(type, data);
    }

    if (type == TABLE_NODE) {
        int cmp = strcmp(((struct Table*)data)->name, node->data.table->name);
        if (cmp < 0) {
            node->left = insert(node->left, type, data);
        } else if (cmp > 0) {
            node->right = insert(node->right, type, data);
        }
    } else if (type == COLUMN_NODE) {
        if (node->firstChild == NULL) {
            node->firstChild = create_node(type, data);
        } else {
            TreeNode* current = node->firstChild;
            while (current->right != NULL) {
                current = current->right;
            }
            current->right = create_node(type, data);
        }
    } else if (type == VALUE_NODE) {
        if (node->firstChild == NULL) {
            node->firstChild = create_node(type, data);
        } else {
            TreeNode* current = node->firstChild;
            while (current->right != NULL) {
                current = current->right;
            }
            current->right = create_node(type, data);
        }
    }

    return node;
}

TreeNode* search(TreeNode* root, const char* tableName) { 
    if (root == NULL || strcmp(tableName, root->data.table->name) == 0) {
        return root;
    }

    if (strcmp(tableName, root->data.table->name) < 0) {
        return search(root->left, tableName);
    } else {
        return search(root->right, tableName);
    }
}

void print_tree(TreeNode* node, int level) {
    if (node == NULL) return;

    for (int i = 0; i < level; i++) printf("  ");

    switch (node->type) {
        case TABLE_NODE:
            printf("|-- %s\n", node->data.table->name);
            break;
        case COLUMN_NODE:
            printf("|-- Column: %s\n", node->data.column->name);
            break;
        case VALUE_NODE:
            printf("|-- Value: %s\n", node->data.value);
            break;
    }

    print_tree(node->firstChild, level + 1);
    print_tree(node->right, level);
}
