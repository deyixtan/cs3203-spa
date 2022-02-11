#ifndef SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_PROCEDURE_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_PROCEDURE_H_

#include "node.h"
#include "node_statement_list.h"

class ProcedureNode : public Node
{
 private:
  std::string m_name;
  std::shared_ptr<StatementListNode> m_stmt_list;

 public:
  ProcedureNode(std::string name, std::shared_ptr<StatementListNode> stmtList);
  std::shared_ptr<StatementListNode> getStatementList();
  std::string getName();
  std::string ToString(int level);
};

#endif //SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_PROCEDURE_H_
