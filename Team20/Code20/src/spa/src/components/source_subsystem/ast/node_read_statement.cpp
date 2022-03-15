#include "node_read_statement.h"

ReadStatementNode::ReadStatementNode(int stmt_no, std::shared_ptr<VariableNode> identifier)
    : StatementNode(stmt_no),
    m_identifier(identifier){}

std::shared_ptr<VariableNode> ReadStatementNode::GetIdentifier() {
  return m_identifier;
}

StmtType ReadStatementNode::GetStatementType() {
  return StmtType::READ;
}

std::string ReadStatementNode::ToString() {
  return StatementNode::ToString() + "read " + m_identifier->ToString() + ";\n";
}

std::string ReadStatementNode::GetPatternFormat() {
  return "";
}

bool ReadStatementNode::operator==(const StatementNode &other) const {
  const auto casted_other = dynamic_cast<const ReadStatementNode*>(&other);
  return m_stmt_no == casted_other->m_stmt_no && *m_identifier == *(casted_other->m_identifier);
}

std::string ReadStatementNode::Process(Populator populator, std::vector<std::string>* visited, std::shared_ptr<source::CfgProcedureNode> cfg_proc_node, std::shared_ptr<source::CfgGroupNode> cfg_node) {
  std::string stmt_num = std::to_string(GetStatementNumber());
  populator.PopulateStmt(stmt_num);
  std::string var_name = m_identifier->GetIdentifier();
  populator.PopulateVars(var_name);
  populator.PopulateRead(stmt_num);
  for (std::string s : *visited) {
    populator.PopulateModifies(s, var_name);
  }
  populator.PopulateModifies(stmt_num, var_name);
  populator.PopulateParentStar(stmt_num, *visited);
  cfg_node->GetNodes().push_back(GetStatementNumber());
  return "";
}
