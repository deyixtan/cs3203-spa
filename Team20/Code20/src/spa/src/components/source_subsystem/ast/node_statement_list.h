#ifndef SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_STATEMENT_LIST_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_STATEMENT_LIST_H_

#include "node.h"
#include "node_statement.h"

class StatementListNode : public Node {
 private:
  std::vector<std::shared_ptr<StatementNode>> m_statements;

 public:
  StatementListNode();
  StatementListNode(std::vector<std::shared_ptr<StatementNode>> statements);
  std::vector<std::shared_ptr<StatementNode>> getStatements();
  std::string ToString(int level);
};

#endif //SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_STATEMENT_LIST_H_
