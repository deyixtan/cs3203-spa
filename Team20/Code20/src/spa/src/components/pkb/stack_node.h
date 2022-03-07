//
// Created by Jaryl Loh on 7/3/22.
//

#ifndef STACK_NODE_H
#define STACK_NODE_H

#include <cstdlib>
#include "tree_node.h"

class StackNode {
 public:
  TreeNode *treeNode;
  StackNode *next;
  /** constructor **/
  StackNode(TreeNode *treeNode)
  {
    this->treeNode = treeNode;
    next = nullptr;
  }
};

#endif //STACK_NODE_H
