#include "node_print_statement.h"
#include "../../iterator/design_extractor.h"
#include "../../iterator/cfg_builder.h"
#include "../cfg/cfg_node.h"

PrintStatementNode::PrintStatementNode(int stmt_no, std::shared_ptr<VariableNode> identifier)
    : StatementNode(stmt_no), m_identifier(identifier) {}

std::shared_ptr<VariableNode> PrintStatementNode::GetIdentifier() {
  return m_identifier;
}

StmtType PrintStatementNode::GetStatementType() {
  return StmtType::PRINT;
}

std::string PrintStatementNode::ToString() {
  return StatementNode::ToString() + "print " + m_identifier->ToString() + ";\n";
}

std::string PrintStatementNode::GetPatternFormat() {
  return "";
}

bool PrintStatementNode::operator==(const StatementNode &other) const {
  const auto casted_other = dynamic_cast<const PrintStatementNode*>(&other);
  return m_stmt_no == casted_other->m_stmt_no && *m_identifier == *(casted_other->m_identifier);
}

void PrintStatementNode::Accept(DesignExtractor *de, std::string proc_name) {
  std::string stmt_num = std::to_string(GetStatementNumber());
  de->GetPkbClient()->PopulateStmt(stmt_num);
  std::string var_name = m_identifier->GetIdentifier();
  de->GetPkbClient()->PopulatePrint(stmt_num, m_identifier->GetIdentifier());
  de->Visit(m_identifier, proc_name, true);
}

std::shared_ptr<CfgNode> PrintStatementNode::Accept(CfgBuilder *cb, std::shared_ptr<CfgNode> cfg_node) {
  cfg_node->AddStatement(std::to_string(GetStatementNumber()));
  return cfg_node;
}
