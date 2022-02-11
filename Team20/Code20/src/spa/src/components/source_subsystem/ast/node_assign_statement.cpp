#include "node_assign_statement.h"

AssignStatementNode::AssignStatementNode(int line,
                                         std::shared_ptr<VariableNode> id,
                                         std::shared_ptr<ExpressionNode> expr)
    : StatementNode(line), m_id(id), m_expression(expr) {}

std::shared_ptr<VariableNode> AssignStatementNode::getId() {
  return m_id;
}

std::shared_ptr<ExpressionNode> AssignStatementNode::getExpr() {
  return m_expression;
}

StmtType AssignStatementNode::getStatementType() {
  return StmtType::ASSIGN;
}

std::string AssignStatementNode::ToString(int level) {
  return StatementNode::ToString(level) + m_id->ToString(level) + " = " + m_expression->ToString(level) + ";\n";
}
