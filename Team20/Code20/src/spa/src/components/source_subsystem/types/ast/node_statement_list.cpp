#include "node_statement_list.h"
#include "components/source_subsystem/pkb_client.h"
#include "../../iterator/design_extractor.h"
#include "../../iterator/cfg_builder.h"
#include "node_statement.h"

namespace source {

StatementListNode::StatementListNode() : m_statements(StatementNodeStream()) {}

StatementListNode::StatementListNode(StatementNodeStream statements)
    : m_statements(std::move(statements)) {}

StatementNodeStream StatementListNode::GetStatements() {
  return m_statements;
}

void StatementListNode::Accept(DesignExtractorPtr design_extractor) {
  design_extractor->Visit(std::dynamic_pointer_cast<StatementListNode>(shared_from_this()));
}

void StatementListNode::Accept(CfgBuilderPtr cfg_builder) {
  cfg_builder->Visit(std::dynamic_pointer_cast<StatementListNode>(shared_from_this()));
}

bool StatementListNode::operator==(const StatementListNode &other) const {
  if (m_statements.size() != other.m_statements.size()) {
    return false;
  }
  for (int i = 0; i < m_statements.size(); i++) {
    if (*(m_statements.at(i)) == *(other.m_statements.at(i))) {}
    else { return false; }
  }
  return true;
}

}
