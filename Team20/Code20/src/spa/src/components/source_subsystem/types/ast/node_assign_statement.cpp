#include "node_assign_statement.h"
#include "../../iterator/design_extractor.h"
#include "../../iterator/cfg_builder.h"
#include "../cfg/cfg_node.h"

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

std::string AssignStatementNode::ToString() {
  return StatementNode::ToString() + m_identifier->ToString() + " = " + m_expression->ToString() + ";\n";
}

std::string AssignStatementNode::GetPatternFormat() {
  return "";
}

bool AssignStatementNode::operator==(const StatementNode &other) const {
  const auto casted_other = dynamic_cast<const AssignStatementNode *>(&other);
  return m_stmt_no == casted_other->m_stmt_no && *m_identifier == *(casted_other->m_identifier)
      && *m_expression == *(casted_other->m_expression);
}

void AssignStatementNode::Accept(DesignExtractor *de, std::string proc_name) {
  std::string stmt_num = std::to_string(GetStatementNumber());
  std::string var_name = "";
  de->GetPkbClient()->PopulateStmt(stmt_num);
  var_name = m_identifier->GetIdentifier();
  de->Visit(m_identifier, proc_name, false);
  std::string rhs_expr = de->Visit(m_expression, proc_name, true);
  de->GetPkbClient()->AddPattern(STMT, stmt_num, var_name, rhs_expr);
  de->GetPkbClient()->PopulateAssign(stmt_num);
  de->GetPkbClient()->PopulateParentStar(stmt_num, de->GetVisited());
}

std::shared_ptr<CfgNode> AssignStatementNode::Accept(CfgBuilder *cb, std::shared_ptr<CfgNode> cfg_node) {
  cfg_node->AddStatement(std::to_string(GetStatementNumber()));
  return cfg_node;
}
