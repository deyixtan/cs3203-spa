#ifndef EXPRESSION_TREE_H
#define EXPRESSION_TREE_H

#include <string>
#include <stack>
#include <iostream>

// Tree Structure
typedef struct node {
  char data;
  struct node *left, *right;
} * nptr;

class ExpressionTree
{
 public:
  ExpressionTree();
  nptr newNode(char c);
  nptr build(std::string& s);
  void postorder(nptr root);
  nptr GetRoot();
  std::string GetPattern(nptr root);
 private:
  nptr root;
};

#endif //EXPRESSION_TREE_H
