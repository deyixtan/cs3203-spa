#include "node_read_statement.h"

ReadStatementNode::ReadStatementNode(int line, std::shared_ptr<VariableNode> id) : StatementNode(line), m_id(id) {}

std::shared_ptr<VariableNode> ReadStatementNode::getId() {
  return m_id;
}

StmtType ReadStatementNode::getStatementType() {
  return StmtType::READ;
}

std::string ReadStatementNode::ToString(int level) {
  return StatementNode::ToString(level) + "read " + m_id->ToString(level) + ";\n";
}
