#ifndef STACK_NODE_H
#define STACK_NODE_H

#include <cstdlib>
#include "tree_node.h"

namespace pkb {

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

}

#endif //STACK_NODE_H
