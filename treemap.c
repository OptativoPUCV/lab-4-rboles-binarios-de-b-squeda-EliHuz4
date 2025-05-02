#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap* Map = (TreeMap *) malloc(sizeof(TreeMap));
    if(Map == NULL) return NULL;
    Map->root = NULL;
    Map->current = NULL;
    Map->lower_than = lower_than;
    return Map;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if(searchTreeMap(tree, key) != NULL) return;
    TreeNode* newNode = createTreeNode(key, value);
    if(newNode == NULL) return;
    TreeNode* parent = NULL;
    TreeNode* current = tree->root;

    while(current != NULL)
    {
        parent = current;

        if (tree->lower_than(key, current->pair->key)) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    newNode->parent = parent;

    if(parent == NULL)
    {
        tree->root = newNode;
    }
    else if(tree->lower_than(key, parent->pair->key))
    {
        parent->left = newNode;
    }
    else
    {
        parent->right = newNode;
    }

    tree->current = newNode;
}

TreeNode * minimum(TreeNode * x){
    TreeNode* Min = x;
    while(Min->left != NULL)
    {
        Min = Min->left;
    }
    return Min;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if (node == NULL) return;

    if (node->left == NULL && node->right == NULL) {
        if (node->parent == NULL) {
            tree->root = NULL;
        } else if (node->parent->left == node) {
            node->parent->left = NULL;
        } else {
            node->parent->right = NULL;
        }
        free(node->pair);
        free(node);
        return;
    }

    if (node->left == NULL || node->right == NULL) {
        TreeNode* hijo;
        if (node->left != NULL) {
            hijo = node->left;
        }
        else {
            hijo = node->right;
        }

        if (node->parent == NULL) {
            tree->root = hijo;
        } else if (node->parent->left == node) {
            node->parent->left = hijo;
        } else {
            node->parent->right = hijo;
        }

        hijo->parent = node->parent;

        free(node->pair);
        free(node);
        return;
    }

    TreeNode* minNode = minimum(node->right);

    node->pair->key = minNode->pair->key;
    node->pair->value = minNode->pair->value;

    removeNode(tree, minNode);
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}

Pair * searchTreeMap(TreeMap* tree, void* key) {
    TreeNode* Aux = tree->root;
    while(Aux != NULL)
    {
        if(tree->lower_than(key, Aux->pair->key))
        {
            Aux = Aux->left;
        }
        else if(tree->lower_than(Aux->pair->key, key))
        {
            Aux = Aux->right;
        }
        else
        {
            tree->current = Aux;
            return Aux->pair;
        }
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    TreeNode* node = tree->root;
    TreeNode* ub_node = NULL;

    while (node != NULL) {
        if (!tree->lower_than(key, node->pair->key) && !tree->lower_than(node->pair->key, key)) {
            return node->pair;
        }

    if (tree->lower_than(key, node->pair->key))
    {
        ub_node = node;
        node = node->left;
    }
    else {
        node = node->right;
    }
    
    if (ub_node != NULL) return ub_node->pair;
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    if(tree->root == NULL) return NULL;
    tree->current = tree->root;

    while(tree->current->left != NULL)
    {
        tree->current = tree->current->left;
    }
    return tree->current->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    if (tree->current == NULL) return NULL;

    TreeNode* node = tree->current;

    if (node->right != NULL) {
        node = minimum(node->right);
        tree->current = node;
        return node->pair;
    }

    TreeNode* parent = node->parent;
    while (parent != NULL && parent->right == node) {
        node = parent;
        parent = parent->parent;
    }

    tree->current = parent;

    if (parent != NULL)
        return parent->pair;
    
    return NULL;
}
