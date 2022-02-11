#include "node_print_statement.h"

PrintStatementNode::PrintStatementNode(int line, std::shared_ptr<VariableNode> id) : StatementNode(line), m_id(id) {}

std::shared_ptr<VariableNode> PrintStatementNode::getId() {
  return m_id;
}

StmtType PrintStatementNode::getStatementType() {
  return StmtType::PRINT;
}

std::string PrintStatementNode::ToString(int level) {
  return StatementNode::ToString(level) + "print " + m_id->ToString(level) + ";\n";
}
