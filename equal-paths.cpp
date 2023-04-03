#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here


bool height_checker(Node* root, int depth, int* first_leaf);

bool equalPaths(Node * root)
{
    // Add your code below

    if(root == nullptr) return true;

    int depth = 0;
    int first_leaf = -1;

    return height_checker(root, depth, &first_leaf);
}

bool height_checker(Node* root, int depth, int* first_leaf){
    if(root == nullptr) return true;

    if(root->left == nullptr && root->right == nullptr){

        if(*first_leaf == -1){
            *first_leaf = depth;

        }
        else{
            if(*first_leaf != depth){
                return false;
            }
            return true;
        }
    }
    return height_checker(root->left, depth+1,first_leaf) && height_checker(root->right, depth+1,first_leaf);
}