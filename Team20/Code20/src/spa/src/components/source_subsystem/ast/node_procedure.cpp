#include "node_procedure.h"

ProcedureNode::ProcedureNode(std::string identifier, std::shared_ptr<StatementListNode> stmt_list) :
    m_identifier(identifier), m_stmt_list(stmt_list) {}

std::string ProcedureNode::GetIdentifier() {
  return m_identifier;
}

std::shared_ptr<StatementListNode> ProcedureNode::GetStatementList() {
  return m_stmt_list;
}

std::string ProcedureNode::ToString(int level) {
  return "procedure " + m_identifier + " {\n" + m_stmt_list->ToString(level + 1) + "}\n";
}

bool ProcedureNode::operator==(const ProcedureNode &other) const {
  std::vector<std::shared_ptr<StatementNode>> this_stmt_list = m_stmt_list->GetStatements();
  std::vector<std::shared_ptr<StatementNode>> other_stmt_list = other.m_stmt_list->GetStatements();
  if (this_stmt_list.size() != other_stmt_list.size()) {
    return false;
  }
  for (int i = 0; i < this_stmt_list.size(); i++) {
    if (*(this_stmt_list.at(i)) == *(other_stmt_list.at(i))) {}
    else { return false; }
  }
  return m_identifier == other.m_identifier;
}

void ProcedureNode::Process(Populator populator, std::vector<std::string>* visited) {
  populator.PopulateProc(m_identifier);
}
