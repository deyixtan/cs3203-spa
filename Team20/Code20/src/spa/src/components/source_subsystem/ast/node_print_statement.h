#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_PRINT_STATEMENT_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_PRINT_STATEMENT_H_

#include "node_statement.h"
#include "node_variable.h"

class PrintStatementNode : public StatementNode {
 private:
  std::shared_ptr<VariableNode> m_identifier;

 public:
  PrintStatementNode(int stmt_no, std::shared_ptr<VariableNode> identifier);
  [[nodiscard]] std::shared_ptr<VariableNode> GetIdentifier();
  [[nodiscard]] StmtType GetStatementType() override;
  [[nodiscard]] std::string ToString() override;
  [[nodiscard]] std::string GetPatternFormat() override;
  [[nodiscard]] bool operator==(const StatementNode &other) const override;
};

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_PRINT_STATEMENT_H_
