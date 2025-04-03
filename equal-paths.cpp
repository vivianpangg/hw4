#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <iostream>
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int findLeafPathDepth(Node *root, int depth){
  if(root ==nullptr){
    return -1; //root/leaf deos not even exist
  }

  if(root->left ==nullptr&& root->right == nullptr){
    return depth; //returns current depth, as it is at the end of the path
  }

  //find left and right paths
  int left = findLeafPathDepth(root->left, depth+1);
  int right = findLeafPathDepth(root->right, depth+1);

  //if one is not valid/or no child then return the other branch
  if(left ==-1){
    return right;
  }else if(right ==-1){
    return left;

  }


  //either should be the same
  return left;

}

bool ifEqualPath(Node *root, int targetDepth, int currentDepth){
  if(root ==nullptr){
    return true; //empty
  }else if(root->left ==nullptr&& root->right == nullptr){
    return targetDepth == currentDepth;
  }else{
    return ifEqualPath(root->left, targetDepth, currentDepth+1)&& ifEqualPath(root->right, targetDepth, currentDepth+1);
  }



}

bool equalPaths(Node * root)
{
    // Add your code below
    if(root ==nullptr){
      return true;
    }else if(root->left ==nullptr && root->right==nullptr){
      return true;

    }
    int leafDepth = findLeafPathDepth(root,0);

    return ifEqualPath(root, leafDepth,0);

}

