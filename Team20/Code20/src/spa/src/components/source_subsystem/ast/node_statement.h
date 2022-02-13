#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_STATEMENT_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_STATEMENT_H_

#include "node.h"

class StatementNode : public Node {
 private:
  int m_stmt_no;

 protected:
  [[nodiscard]] std::string GetStatementLabel();

 public:
  StatementNode(int stmt_no);
  [[nodiscard]] int GetStatementNumber();
  [[nodiscard]] virtual StmtType GetStatementType() = 0;
  [[nodiscard]] std::string ToString(int level) override;
};

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_STATEMENT_H_
