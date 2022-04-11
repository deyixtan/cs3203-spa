#include "node_procedure.h"
#include "components/source_subsystem/iterator/design_extractor.h"
#include "components/source_subsystem/iterator/cfg_builder.h"
#include "components/source_subsystem/types/ast/node_statement.h"
#include "components/source_subsystem/types/ast/node_statement_list.h"

namespace source {

ProcedureNode::ProcedureNode(String name, StatementListNodePtr stmt_list) :
    m_name(std::move(name)), m_stmt_list(std::move(stmt_list)) {}

String ProcedureNode::GetName() {
  return m_name;
}

StatementListNodePtr ProcedureNode::GetStatementList() {
  return m_stmt_list;
}

void ProcedureNode::Accept(DesignExtractorPtr design_extractor) {
  ProcedureNodePtr derived_ptr = std::dynamic_pointer_cast<ProcedureNode>(shared_from_this());
  design_extractor->Visit(derived_ptr);
}

void ProcedureNode::Accept(CfgBuilderPtr cfg_builder) {
  ProcedureNodePtr derived_ptr = std::dynamic_pointer_cast<ProcedureNode>(shared_from_this());
  cfg_builder->Visit(derived_ptr);
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
