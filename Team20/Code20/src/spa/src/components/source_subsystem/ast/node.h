#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_H_

// TODO: remove dependence on PKB's StmtType
#include "../../pkb/pkb.h"
#include "../populator.h"

class Node {
 public:
  virtual std::string Process(Populator populator, std::vector<std::string> *visited, std::shared_ptr<source::CfgGroupNode> cfg_node) = 0;
  [[nodiscard]] virtual std::string ToString() = 0;
  [[nodiscard]] virtual std::string GetPatternFormat() = 0;
};

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_H_
