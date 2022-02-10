#include "node_statement.h"

StatementNode::StatementNode(int line) : m_line_number(line) {}

std::vector<std::shared_ptr<StatementNode>> StatementNode::getStatementList() {
  return {};
}

std::string StatementNode::getStatementLabel() {
  if (m_line_number == 0) {
    return "   ";
  } else {
    std::string num = std::to_string(m_line_number);
    return std::string(3 - num.length(), ' ') + num;
  }
}

StmtType StatementNode::getStatementType() {
  return StmtType::NONE;
}

int StatementNode::getLineNumber() {
  return m_line_number;
}

std::string StatementNode::format(int level) {
  return this->getStatementLabel();
}
