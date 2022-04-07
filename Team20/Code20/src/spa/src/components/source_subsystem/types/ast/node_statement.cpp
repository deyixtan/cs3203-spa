#include "node_statement.h"

namespace source {

StatementNode::StatementNode(String stmt_no) : m_stmt_no(std::move(stmt_no)) {}

String StatementNode::GetStatementNumber() {
  return m_stmt_no;
}

}
