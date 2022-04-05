#include "node_assign_statement.h"
#include "../../iterator/design_extractor.h"
#include "../../iterator/cfg_builder.h"
#include "../cfg/cfg_node.h"
#include "node_variable.h"
#include "node_constant.h"
#include "node_combination_expression.h"

namespace source {

AssignStatementNode::AssignStatementNode(String stmt_no, VariableNodePtr lhs, ExpressionNodePtr rhs)
    : StatementNode(stmt_no), m_lhs(lhs), m_rhs(rhs) {}

VariableNodePtr AssignStatementNode::GetLhs() {
  return m_lhs;
}

ExpressionNodePtr AssignStatementNode::GetRhs() {
  return m_rhs;
}

void AssignStatementNode::Accept(DesignExtractor *de, String proc_name) {
  String stmt_num = GetStatementNumber();
  de->GetPkbClient()->PopulateTypeOfStmt(stmt_num, ASSIGN);
  String var_name = m_lhs->GetIdentifier();
  de->Visit(m_lhs, proc_name, false);
  String rhs_expr = de->Visit(m_rhs, proc_name, true);
  de->GetPkbClient()->PopulateAssign(de->GetVisited(), proc_name, stmt_num, var_name, rhs_expr);
}

CfgNodePtr AssignStatementNode::Accept(CfgBuilder *cb, CfgNodePtr cfg_node) {
  cfg_node->AddStatement(StmtType::ASSIGN, GetStatementNumber());
  return cfg_node;
}

bool AssignStatementNode::operator==(const StatementNode &other) const {
  const auto casted_other = dynamic_cast<const AssignStatementNode *>(&other);
  return m_stmt_no == casted_other->m_stmt_no && *m_lhs == *(casted_other->m_lhs)
      && *m_rhs == *(casted_other->m_rhs);
}

}
