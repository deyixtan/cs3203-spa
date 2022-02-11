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
