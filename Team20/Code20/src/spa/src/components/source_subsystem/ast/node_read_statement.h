#ifndef SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_READ_STATEMENT_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_READ_STATEMENT_H_

#include "node_statement.h"
#include "node_variable.h"

class ReadStatementNode : public StatementNode
{
 private:
  std::shared_ptr<VariableNode> m_id;

 public:
  ReadStatementNode(int line, std::shared_ptr<VariableNode> id);
  std::shared_ptr<VariableNode> getId();
  std::string ToString(int level);
  StmtType getStatementType();
};

#endif //SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_READ_STATEMENT_H_
