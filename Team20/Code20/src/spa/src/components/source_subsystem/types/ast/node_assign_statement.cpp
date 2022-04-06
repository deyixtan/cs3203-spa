#include "node_assign_statement.h"
#include "components/source_subsystem/pkb_client.h"
#include "components/source_subsystem/iterator/design_extractor.h"
#include "components/source_subsystem/types/ast/node_variable.h"
#include "components/source_subsystem/types/cfg/cfg_node.h"

namespace source {

AssignStatementNode::AssignStatementNode(String &stmt_no, VariableNodePtr lhs, ExpressionNodePtr rhs)
    : StatementNode(stmt_no), m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}

VariableNodePtr AssignStatementNode::GetLhs() {
  return m_lhs;
}

ExpressionNodePtr AssignStatementNode::GetRhs() {
  return m_rhs;
}

void AssignStatementNode::Accept(DesignExtractor *design_extractor, String proc_name) {
  String stmt_num = GetStatementNumber();
  design_extractor->GetPkbClient()->PopulateTypeOfStmt(stmt_num, ASSIGN);
  String var_name = m_lhs->GetName();
  design_extractor->Visit(m_lhs, proc_name, false);
  String rhs_expr = design_extractor->Visit(m_rhs, proc_name, true);
  design_extractor->GetPkbClient()->PopulateAssign(design_extractor->GetVisited(), proc_name, stmt_num, var_name, rhs_expr);
}

CfgNodePtr AssignStatementNode::Accept(CfgBuilder *cfg_builder, CfgNodePtr cfg_node) {
  cfg_node->AddStatement(StmtType::ASSIGN, GetStatementNumber());
  return cfg_node;
}

bool AssignStatementNode::operator==(const StatementNode &other) const {
  const auto casted_other = dynamic_cast<const AssignStatementNode *>(&other);
  return m_stmt_no == casted_other->m_stmt_no && *m_lhs == *(casted_other->m_lhs)
      && *m_rhs == *(casted_other->m_rhs);
}

StmtType AssignStatementNode::GetStatementType() {
  return StmtType::ASSIGN;
}

}
