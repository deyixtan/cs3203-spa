#include "node_procedure.h"
#include "components/source_subsystem/pkb_client.h"
#include "../../iterator/design_extractor.h"
#include "../../iterator/cfg_builder.h"
#include "node_statement_list.h"
#include "node_statement.h"

namespace source {

ProcedureNode::ProcedureNode(String name, StatementListNodePtr stmt_list) :
    m_name(std::move(name)), m_stmt_list(std::move(stmt_list)) {}

String ProcedureNode::GetName() {
  return m_name;
}

StatementListNodePtr ProcedureNode::GetStatementList() {
  return m_stmt_list;
}

void ProcedureNode::Accept(DesignExtractor *design_extractor) {
  design_extractor->GetPkbClient()->PopulateProc(m_name);
  design_extractor->Visit(m_stmt_list, m_name);
}

CfgNodePtr ProcedureNode::Accept(CfgBuilder *cfg_builder, CfgNodePtr cfg_node) {
  return cfg_builder->Visit(m_stmt_list, std::move(cfg_node));
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
  return m_name == other.m_name;
}

}
