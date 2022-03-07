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
  std::stack<nptr> stN;

  // Stack to hold chars
  std::stack<char> stC;
  nptr t, t1, t2;

  // Prioritising the operators
  int p[123] = { 0 };
  p['+'] = p['-'] = 1, p['/'] = p['*'] = 2, p['^'] = 3,
  p[')'] = 0;

  for (int i = 0; i < s.length(); i++) {
    if (s[i] == '(') {

      // Push '(' in char stack
      stC.push(s[i]);
    }

      // Push the operands in node stack
    else if (isalnum(s[i])) {
      t = newNode(s[i]);
      stN.push(t);
    } else if (p[s[i]] > 0) {
      // If an operator with lower or
      // same associativity appears
      while (
          !stC.empty() && stC.top() != '('
              && ((s[i] != '^' && p[stC.top()] >= p[s[i]])
                  || (s[i] == '^'
                      && p[stC.top()] > p[s[i]])))
      {

        // Get and remove the top element
        // from the character stack
        t = newNode(stC.top());
        stC.pop();

        // Get and remove the top element
        // from the node stack
        t1 = stN.top();
        stN.pop();

        // Get and remove the currently top
        // element from the node stack
        t2 = stN.top();
        stN.pop();

        // Update the tree
        t->left = t2;
        t->right = t1;

        // Push the node to the node stack
        stN.push(t);
      }

      // Push s[i] to char stack
      stC.push(s[i]);
    }
    else if (s[i] == ')') {
      while (!stC.empty() && stC.top() != '(')
      {
        t = newNode(stC.top());
        stC.pop();
        t1 = stN.top();
        stN.pop();
        t2 = stN.top();
        stN.pop();
        t->left = t2;
        t->right = t1;
        stN.push(t);
      }
      stC.pop();
    }
  }
  this->root = stN.top();
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