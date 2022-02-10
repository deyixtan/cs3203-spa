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

std::string AssignStatementNode::format(int level) {
  return StatementNode::format(level) + m_id->format(level) + " = " + m_expression->format(level) + ";\n";
}
