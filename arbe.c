#include "arbre.h"


struct TreeNode *root = NULL;



TreeNode* create_node(struct Table *table) { // Create a new node
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode)); // Allocate memory for new node
    newNode->table = table; // Assign table to node
    newNode->left = newNode->right = NULL; // Initialize left and right children as NULL
    return newNode; // Return the new node
}

TreeNode* insert(TreeNode* node, struct Table *table) { // Insert a new table into the tree
    if (node == NULL) {
        return create_node(table);
    } // If the tree is empty, return a new node
 
    if (strcmp(table->name, node->table->name) < 0) {
                                                     // If the table name is less than the current node, insert it into the left subtree
        node->left = insert(node->left, table); 
        }  
    else if (strcmp(table->name, node->table->name) > 0)
     {
        node->right = insert(node->right, table); // If the table name is greater than the current node, insert it into the right subtree
     }
    return node; 
}

TreeNode* search(TreeNode* root, const char* tableName) { // Search for a table in the tree
    if (root == NULL || strcmp(tableName, root->table->name) == 0){ // If the tree is empty or the table is found, return the root
        return root;}

    if (strcmp(tableName, root->table->name) < 0) {
        return search(root->left, tableName); 
        }// If the table name is less than the current node, search the left subtree

    return search(root->right, tableName); // If the table name is greater than the current node, search the right subtree
}

