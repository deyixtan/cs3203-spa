#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_READ_STATEMENT_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_READ_STATEMENT_H_

#include "node_statement.h"
#include "node_variable.h"

class ReadStatementNode : public StatementNode {
 private:
  std::shared_ptr<VariableNode> m_identifier;

 public:
  ReadStatementNode(int stmt_no, std::shared_ptr<VariableNode> identifier);
  [[nodiscard]] std::shared_ptr<VariableNode> GetIdentifier();
  [[nodiscard]] StmtType GetStatementType() override;
  [[nodiscard]] std::string ToString(int level) override;
};

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_READ_STATEMENT_H_
