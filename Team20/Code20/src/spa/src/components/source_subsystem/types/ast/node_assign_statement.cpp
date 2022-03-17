#include "node_assign_statement.h"
#include "../../iterator/design_extractor.h"

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

std::string AssignStatementNode::Process(Populator populator, std::vector<std::string> *visited, bool is_uses, std::shared_ptr<source::CfgProcedureNode> cfg_proc_node, std::shared_ptr<source::CfgGroupNode> cfg_node) {
//  std::string stmt_num = std::to_string(GetStatementNumber());
//  std::string var_name = "";
//  de->GetPopulator()->PopulateStmt(stmt_num);
//  var_name = m_identifier->GetIdentifier();
//  m_identifier->Process(populator, visited, false, cfg_proc_node, cfg_node);
//  std::string rhs_expr = m_expression->Process(populator, visited, true, cfg_proc_node, cfg_node);
//  de->GetPopulator()->AddStmtPattern(stmt_num, var_name, rhs_expr);
//  de->GetPopulator()->PopulateAssign(stmt_num);
//  de->GetPopulator()->PopulateParentStar(stmt_num, *visited);
//  if (cfg_node == nullptr) {
//    cfg_proc_node->GetLastNode()->GetNodes().emplace_back(GetStatementNumber());
//  } else {
//    cfg_node->GetNodes().emplace_back(GetStatementNumber());
//  }
  return "";
}

void AssignStatementNode::Accept(DesignExtractor *de) {
  std::string stmt_num = std::to_string(GetStatementNumber());
  std::string var_name = "";
  de->GetPopulator()->PopulateStmt(stmt_num);
  var_name = m_identifier->GetIdentifier();
  de->Visit(m_identifier, false);
  //std::string rhs_expr = m_expression->Process(populator, visited, true, cfg_proc_node, cfg_node);
  std::string rhs_expr = de->Visit(m_expression, true);
  de->GetPopulator()->AddStmtPattern(stmt_num, var_name, rhs_expr);
  de->GetPopulator()->PopulateAssign(stmt_num);
  de->GetPopulator()->PopulateParentStar(stmt_num, de->GetVisited());
}