#include "node_procedure.h"

ProcedureNode::ProcedureNode(std::string name, std::shared_ptr<StatementListNode> stmtList) :
    m_name(name), m_stmt_list(stmtList) {}

std::string ProcedureNode::getName() {
  return m_name;
}

std::shared_ptr<StatementListNode> ProcedureNode::getStatementList() {
  return m_stmt_list;
}

std::string ProcedureNode::ToString(int level) {
  return "procedure " + m_name + " {\n" + m_stmt_list->ToString(level + 1) + "}\n";
}
