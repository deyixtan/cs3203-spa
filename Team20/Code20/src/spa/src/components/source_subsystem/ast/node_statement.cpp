#include "node_statement.h"

StatementNode::StatementNode(int stmt_no) : m_stmt_no(stmt_no) {}

int StatementNode::GetStatementNumber() {
  return m_stmt_no;
}

std::string StatementNode::GetStatementLabel() {
  if (m_stmt_no == 0) {
    return "   ";
  }

  std::string num = std::to_string(m_stmt_no);
  return std::string(3 - num.length(), ' ') + num;
}

std::string StatementNode::ToString(int level) {
  return GetStatementLabel();
}
