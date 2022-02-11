#ifndef SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_STATEMENT_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_STATEMENT_H_

#include "node.h"

class StatementNode : public Node
{
 private:
  int m_line_number = 0;

 protected:
  std::string getStatementLabel();

 public:
  StatementNode(int line);

  virtual std::vector<std::shared_ptr<StatementNode>> getStatementList();
  virtual StmtType getStatementType();
  int getLineNumber();
  std::string ToString(int level);
};


#endif //SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_STATEMENT_H_
