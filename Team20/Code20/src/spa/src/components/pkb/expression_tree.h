#ifndef EXPRESSION_TREE_H
#define EXPRESSION_TREE_H

#include <string>
#include <stack>
#include <iostream>
#include <sstream>
#include <vector>

// Tree Structure
typedef struct node {
  std::string data;
  struct node *left, *right;
} * nptr;

class ExpressionTree
{
 public:
  ExpressionTree();
  nptr newNode(std::string c);
  nptr build(std::string& s);
  void postorder(nptr root);
  nptr GetRoot();
  std::string GetPattern(nptr root);
  std::vector<std::string> split(const std::string &s, char delim);
 private:
  nptr root;
};

#endif //EXPRESSION_TREE_H
