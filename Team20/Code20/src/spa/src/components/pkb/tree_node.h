//
// Created by Jaryl Loh on 7/3/22.
//

#ifndef SPA_SRC_SPA_SRC_COMPONENTS_PKB_TREE_NODE_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_PKB_TREE_NODE_H_

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

#endif //SPA_SRC_SPA_SRC_COMPONENTS_PKB_TREE_NODE_H_
