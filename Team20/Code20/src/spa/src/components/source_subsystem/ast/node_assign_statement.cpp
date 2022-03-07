#include "node_assign_statement.h"

AssignStatementNode::AssignStatementNode(int stmt_no,
                                         std::shared_ptr<VariableNode> identifier,
                                         std::shared_ptr<ExpressionNode> expression)
    : StatementNode(stmt_no), m_identifier(identifier), m_expression(expression) {}

std::shared_ptr<VariableNode> AssignStatementNode::GetIdentifier() {
  return m_identifier;
}

std::shared_ptr<ExpressionNode> AssignStatementNode::GetExpression() {
  return m_expression;
}

StmtType AssignStatementNode::GetStatementType() {
  return StmtType::ASSIGN;
}

std::string AssignStatementNode::ToString(int level) {
  return StatementNode::ToString(level) + m_identifier->ToString(level) + " = " + m_expression->ToString(level) + ";\n";
}

bool AssignStatementNode::operator==(const StatementNode &other) const {
  const auto casted_other = dynamic_cast<const AssignStatementNode *>(&other);
  return m_stmt_no == casted_other->m_stmt_no && *m_identifier == *(casted_other->m_identifier)
      && *m_expression == *(casted_other->m_expression);
}

void AssignStatementNode::Process(Populator populator, std::vector<std::string>* visited) {
  std::string stmt_num = std::to_string(GetStatementNumber());
  std::string var_name = "";
  populator.PopulateStmt(stmt_num);
  var_name = m_identifier->GetIdentifier();
  populator.PopulateVars(var_name);
  for (std::string s : *visited) {
    populator.PopulateModifies(s, var_name);
  }
  populator.PopulateModifies(stmt_num, var_name);

//  std::string rhs_expr = ExprNodeHandler(visited, stmt_num, m_expression, 0, "");
//  rhs_expr.erase(0, 1);
//  populator.AddPattern(stmt_num, var_name, rhs_expr);

  populator.PopulateAssign(stmt_num);
  populator.PopulateParentStar(stmt_num, *visited);
}
