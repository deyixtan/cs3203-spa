#include "expression_tree.h"

ExpressionTree::ExpressionTree() {}

// Function to create new node
nptr ExpressionTree::newNode(char c) {
  nptr n = new node;
  n->data = c;
  n->left = n->right = nullptr;
  return n;
}

// Function to build Expression Tree
nptr ExpressionTree::build(std::string& s) {
  // Stack to hold nodes
  std::stack<nptr> node_stack;

  // Stack to hold chars
  std::stack<char> char_stack;
  nptr tree, tree1, tree2;

  // Prioritising the operators
  int p[123] = { 0 };
  p['+'] = p['-'] = 1, p['/'] = p['*'] = 2, p['^'] = 3,
  p[')'] = 0;

  for (int i = 0; i < s.length(); i++) {
    if (s[i] == '(') {

      // Push '(' in char stack
      char_stack.push(s[i]);
    }

      // Push the operands in node stack
    else if (isalnum(s[i])) {
      tree = newNode(s[i]);
      node_stack.push(tree);
    } else if (p[s[i]] > 0) {
      // If an operator with lower or
      // same associativity appears
      while (
          !char_stack.empty() && char_stack.top() != '('
              && ((s[i] != '^' && p[char_stack.top()] >= p[s[i]])
                  || (s[i] == '^'
                      && p[char_stack.top()] > p[s[i]])))
      {

        // Get and remove the top element
        // from the character stack
        tree = newNode(char_stack.top());
        char_stack.pop();

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

      // Push s[i] to char stack
      char_stack.push(s[i]);
    }
    else if (s[i] == ')') {
      while (!char_stack.empty() && char_stack.top() != '(')
      {
        tree = newNode(char_stack.top());
        char_stack.pop();
        tree1 = node_stack.top();
        node_stack.pop();
        tree2 = node_stack.top();
        node_stack.pop();
        tree->left = tree2;
        tree->right = tree1;
        node_stack.push(tree);
      }
      char_stack.pop();
    }
  }
  this->root = node_stack.top();
  return this->root;
}

// Function to print the post order
// traversal of the tree
void ExpressionTree::postorder(nptr root)
{
  if (root)
  {
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
  std::string temp;
  temp.push_back(root->data);

  if (root->data == '-' || root->data == '+' || root->data == '*' || root->data == '/') {
    pattern += "(" + GetPattern(root->left) + temp + GetPattern(root->right) + ")";
  } else {
    return "(" + temp + ")";
  }

  return pattern;
}