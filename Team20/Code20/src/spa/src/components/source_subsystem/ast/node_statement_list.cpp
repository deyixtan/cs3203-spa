#include "node_statement_list.h"

StatementListNode::StatementListNode() : m_statements(std::vector<std::shared_ptr<StatementNode >>()) {}

StatementListNode::StatementListNode(std::vector<std::shared_ptr<StatementNode>> statements)
    : m_statements(statements) {}

std::vector<std::shared_ptr<StatementNode>> StatementListNode::getStatements() {
  return m_statements;
}

std::string StatementListNode::format(int level) {
  std::string acc = "";
  for (std::shared_ptr<StatementNode> statement : m_statements) {
    acc += statement->format(level);
  }
  return acc;
}
