#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_CALL_STATEMENT_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_CALL_STATEMENT_H_

#include "node_statement.h"

class CallStatementNode : public StatementNode {
 private:
  std::string m_identifier;

 public:
  CallStatementNode(int stmt_no, std::string m_identifier);
  [[nodiscard]] std::string GetIdentifier();
  [[nodiscard]] StmtType GetStatementType() override;
  [[nodiscard]] std::string ToString() override;
  [[nodiscard]] bool operator==(const StatementNode &other) const override;
};

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_CALL_STATEMENT_H_
