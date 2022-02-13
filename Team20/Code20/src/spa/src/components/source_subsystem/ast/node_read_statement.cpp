#include "node_read_statement.h"

ReadStatementNode::ReadStatementNode(int stmt_no, std::shared_ptr<VariableNode> identifier)
    : StatementNode(stmt_no), m_identifier(identifier) {}

std::shared_ptr<VariableNode> ReadStatementNode::GetIdentifier() {
  return m_identifier;
}

StmtType ReadStatementNode::GetStatementType() {
  return StmtType::READ;
}

std::string ReadStatementNode::ToString(int level) {
  return StatementNode::ToString(level) + "read " + m_identifier->ToString(level) + ";\n";
}

bool ReadStatementNode::operator==(const StatementNode &other) const {
  const auto casted_other = dynamic_cast<const ReadStatementNode*>(&other);
  return m_stmt_no == casted_other->m_stmt_no && *m_identifier == *(casted_other->m_identifier);
}
