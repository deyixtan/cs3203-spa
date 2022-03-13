#include "node_print_statement.h"

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

void PrintStatementNode::Process(Populator populator, std::vector<std::string>* visited) {
  std::string stmt_num = std::to_string(GetStatementNumber());
  populator.PopulateStmt(stmt_num);
  std::string var_name = m_identifier->GetIdentifier();
  populator.PopulateVars(var_name);
  populator.PopulatePrint(stmt_num);
  for (std::string s : *visited) {
    populator.PopulateUses(s, var_name);
  }
  populator.PopulateUses(stmt_num, var_name);
}

void PrintStatementNode::Process(Populator populator, std::vector<std::string> *visited, std::string stmt) {}

std::string PrintStatementNode::Process(Populator populator, std::vector<std::string> *visited, std::string stmt_num, int direction, std::string pattern) {}
