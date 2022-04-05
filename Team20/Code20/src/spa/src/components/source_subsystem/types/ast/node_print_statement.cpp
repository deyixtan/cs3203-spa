#include "node_print_statement.h"
#include "../../iterator/design_extractor.h"
#include "../../iterator/cfg_builder.h"
#include "node_variable.h"

namespace source {

PrintStatementNode::PrintStatementNode(String stmt_no, VariableNodePtr identifier)
    : StatementNode(stmt_no), m_identifier(identifier) {}

VariableNodePtr PrintStatementNode::GetIdentifier() {
  return m_identifier;
}

StmtType PrintStatementNode::GetStatementType() {
  return StmtType::PRINT;
}

void PrintStatementNode::Accept(DesignExtractor *de, std::string proc_name) {
  String stmt_num = GetStatementNumber();
  String var_name = m_identifier->GetIdentifier();
  de->GetPkbClient()->PopulateTypeOfStmt(stmt_num, PRINT);

  de->Visit(m_identifier, proc_name, true);
  de->GetPkbClient()->PopulatePrint(de->GetVisited(), stmt_num, var_name);
}

CfgNodePtr PrintStatementNode::Accept(CfgBuilder *cb, CfgNodePtr cfg_node) {
  cfg_node->AddStatement(StmtType::PRINT, GetStatementNumber());
  return cfg_node;
}

bool PrintStatementNode::operator==(const StatementNode &other) const {
  const auto casted_other = dynamic_cast<const PrintStatementNode *>(&other);
  return m_stmt_no == casted_other->m_stmt_no && *m_identifier == *(casted_other->m_identifier);
}

}
