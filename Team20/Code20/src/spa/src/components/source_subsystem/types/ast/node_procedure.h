#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_PROCEDURE_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_PROCEDURE_H_

#include "node.h"

namespace source {

class ProcedureNode : public Node {
 private:
  std::string m_identifier;
  std::shared_ptr<StatementListNode> m_stmt_list;

 public:
  ProcedureNode(std::string identifier, std::shared_ptr<StatementListNode> stmt_list);
  [[nodiscard]] std::string GetIdentifier();
  [[nodiscard]] std::shared_ptr<StatementListNode> GetStatementList();
  [[nodiscard]] std::string ToString() override;
  [[nodiscard]] std::string GetPatternFormat() override;
  [[nodiscard]] bool operator==(const ProcedureNode &other) const;
  void Accept(DesignExtractor *de);
  std::shared_ptr<CfgNode> Accept(CfgBuilder *cb, std::shared_ptr<CfgNode> cfg_node);
};

}

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_PROCEDURE_H_
