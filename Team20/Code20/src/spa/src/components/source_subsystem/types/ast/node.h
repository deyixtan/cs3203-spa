#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_H_

// TODO: remove dependence on PKB's StmtType
#include "components/pkb/pkb.h"
#include "components/source_subsystem/populator.h"

class Node {
 public:
  virtual std::string Process(Populator populator, std::vector<std::string> *visited, bool is_uses, std::shared_ptr<source::CfgProcedureNode> cfg_proc_node, std::shared_ptr<source::CfgGroupNode> cfg_node, std::string proc_name) = 0;
  [[nodiscard]] virtual std::string ToString() = 0;
  [[nodiscard]] virtual std::string GetPatternFormat() = 0;
};

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_H_
