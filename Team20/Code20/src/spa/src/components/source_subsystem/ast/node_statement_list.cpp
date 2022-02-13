#include "node_statement_list.h"

StatementListNode::StatementListNode() : m_statements(std::vector<std::shared_ptr<StatementNode >>()) {}

StatementListNode::StatementListNode(std::vector<std::shared_ptr<StatementNode>> statements)
    : m_statements(statements) {}

std::vector<std::shared_ptr<StatementNode>> StatementListNode::GetStatements() {
  return m_statements;
}

std::string StatementListNode::ToString(int level) {
  std::string str = "";
  for (std::shared_ptr<StatementNode> statement : m_statements) {
    str += statement->ToString(level);
  }
  return str;
}
