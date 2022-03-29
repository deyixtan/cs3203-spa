#ifndef TREE_NODE_H
#define TREE_NODE_H

#include <cstdlib>

class TreeNode
{
 public:
  char data;
  TreeNode *left, *right;
  /** constructor **/
  TreeNode(char data)
  {
    this->data = data;
    this->left = nullptr;
    this->right = nullptr;
  }
};

#endif //TREE_NODE_H
