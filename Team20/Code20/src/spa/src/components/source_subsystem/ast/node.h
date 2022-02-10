#ifndef NODE_H
#define NODE_H

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>
#include "components/pkb/pkb.h"

class Node {
 public:
  virtual std::string format(int level) = 0;
};

#endif //NODE_H
