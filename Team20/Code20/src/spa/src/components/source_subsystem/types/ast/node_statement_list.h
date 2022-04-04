#ifndef SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_STATEMENT_LIST_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_STATEMENT_LIST_H_

#include "node.h"

namespace source {

class StatementListNode : public Node {
 private:
  std::vector<std::shared_ptr<StatementNode>> m_statements;

 public:
  StatementListNode();
  StatementListNode(std::vector<std::shared_ptr<StatementNode>> statements);
  [[nodiscard]] std::vector<std::shared_ptr<StatementNode>> GetStatements();
  [[nodiscard]] std::string ToString() override;
  [[nodiscard]] std::string GetPatternFormat() override;
  [[nodiscard]] bool operator==(const StatementListNode &other) const;
  void Accept(DesignExtractor *de, std::string proc_name);
  std::shared_ptr<CfgNode> Accept(CfgBuilder *cb, std::shared_ptr<CfgNode> cfg_node);
};

}

#endif //SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_STATEMENT_LIST_H_
