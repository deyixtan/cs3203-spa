#include "node_procedure.h"
#include "components/source_subsystem/pkb_client.h"
#include "../../iterator/design_extractor.h"
#include "../../iterator/cfg_builder.h"
#include "node_statement_list.h"
#include "node_statement.h"

namespace source {

ProcedureNode::ProcedureNode(String identifier, StatementListNodePtr stmt_list) :
    m_identifier(identifier), m_stmt_list(stmt_list) {}

String ProcedureNode::GetIdentifier() {
  return m_identifier;
}

StatementListNodePtr ProcedureNode::GetStatementList() {
  return m_stmt_list;
}

bool ProcedureNode::operator==(const ProcedureNode &other) const {
  StatementNodeStream this_stmt_list = m_stmt_list->GetStatements();
  StatementNodeStream other_stmt_list = other.m_stmt_list->GetStatements();
  if (this_stmt_list.size() != other_stmt_list.size()) {
    return false;
  }
  for (int i = 0; i < this_stmt_list.size(); i++) {
    if (*(this_stmt_list.at(i)) == *(other_stmt_list.at(i))) {}
    else { return false; }
  }
  return m_identifier == other.m_identifier;
}

void ProcedureNode::Accept(DesignExtractor *de) {
  de->GetPkbClient()->PopulateProc(m_identifier);
  de->Visit(m_stmt_list, m_identifier);
}

CfgNodePtr ProcedureNode::Accept(CfgBuilder *cb, CfgNodePtr cfg_node) {
  return cb->Visit(m_stmt_list, cfg_node);
}

}
