#include "expression_tree.h"

ExpressionTree::ExpressionTree() {}

// Function to create new node
nptr ExpressionTree::newNode(std::string c) {
  nptr n = new node;
  n->data = c;
  n->left = n->right = nullptr;
  return n;
}

// Function to build Expression Tree
nptr ExpressionTree::build(std::string& s) {
  // Stack to hold nodes
  std::stack<nptr> node_stack;
  std::string data = "";
  char prev = NULL;

  // Stack to hold chars
  std::stack<std::string> str_stack;
  nptr tree, tree1, tree2;

  // Prioritising the operators
  int p[123] = { 0 };
  p['+'] = p['-'] = 1, p['/'] = p['*'] = p['%'] = 2,
  p[')'] = 0;

  for (int i = 0; i < s.length(); i++) {
    std::string str;
    str.push_back(s[i]);
    
    if (str == "(") {
      // Push '(' in char stack
      str_stack.push(str);
    } else if (isalnum(s[i])) { // Push the operands in node stack
      if (isalnum(prev) || prev == NULL) {
        data += str;
      } else {
        data = str;
      }
    } else if (p[s[i]] > 0) {
      // If an operator with lower or
      // same associativity appears
      if (data != "") {
        tree = newNode(data);
        node_stack.push(tree);
        data = "";
      }

      while (!str_stack.empty()
              && str_stack.top() != "("
              && p[str_stack.top()[0]] >= p[s[i]]) {

        // Get and remove the top element
        // from the character stack
        tree = newNode(str_stack.top());
        str_stack.pop();

        // Get and remove the top element
        // from the node stack
        tree1 = node_stack.top();
        node_stack.pop();

        // Get and remove the currently top
        // element from the node stack
        tree2 = node_stack.top();
        node_stack.pop();

        // Update the tree
        tree->left = tree2;
        tree->right = tree1;

        // Push the node to the node stack
        node_stack.push(tree);
      }

      // Push str to char stack
      str_stack.push(str);
    } else if (str == ")") {
      if (isalnum(prev) && data != "") {
        tree = newNode(data);
        node_stack.push(tree);
        data = "";
      }

      while (!str_stack.empty() && str_stack.top() != "(") {
        tree = newNode(str_stack.top());
        str_stack.pop();
        tree1 = node_stack.top();
        node_stack.pop();
        tree2 = node_stack.top();
        node_stack.pop();
        tree->left = tree2;
        tree->right = tree1;
        node_stack.push(tree);
      }
      str_stack.pop();

    }
    prev = s[i];
  }
  this->root = node_stack.top();
  return this->root;
}

// Function to print the post order
// traversal of the tree
void ExpressionTree::postorder(nptr root) {
  if (root) {
    postorder(root->left);
    std::cout << root->data;
    postorder(root->right);
  }
}

nptr ExpressionTree::GetRoot() {
  return this->root;
}

std::string ExpressionTree::GetPattern(nptr root) {
  std::string pattern = "";

  if (root->data == "-" || root->data == "+" || root->data == "*" || root->data == "/") {
    pattern += "(" + GetPattern(root->left) + root->data + GetPattern(root->right) + ")";
  } else {
    return "(" + root->data + ")";
  }

  return pattern;
}