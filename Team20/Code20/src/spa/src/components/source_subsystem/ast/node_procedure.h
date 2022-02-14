#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_PROCEDURE_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_PROCEDURE_H_

#include "node.h"
#include "node_statement_list.h"

class ProcedureNode : public Node {
 private:
  std::string m_identifier;
  std::shared_ptr<StatementListNode> m_stmt_list;

 public:
  ProcedureNode(std::string identifier, std::shared_ptr<StatementListNode> stmt_list);
  [[nodiscard]] std::string GetIdentifier();
  [[nodiscard]] std::shared_ptr<StatementListNode> GetStatementList();
  [[nodiscard]] std::string ToString(int level) override;
  [[nodiscard]] bool operator==(const ProcedureNode &other) const;
};

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_PROCEDURE_H_
