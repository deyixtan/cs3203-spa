#include "node_while_statement.h"
#include "components/source_subsystem/iterator/cfg_builder.h"
#include "components/source_subsystem/iterator/design_extractor.h"
#include "components/source_subsystem/types/ast/node_conditional_expression.h"
#include "components/source_subsystem/types/ast/node_statement_list.h"

namespace source {

WhileStatementNode::WhileStatementNode(String &stmt_no, ConditionalExpressionNodePtr condition, StatementListNodePtr stmt_list)
    : StatementNode(stmt_no), m_condition(std::move(condition)), m_stmt_list(std::move(stmt_list)) {}

ConditionalExpressionNodePtr WhileStatementNode::GetCondition() {
  return m_condition;
}

StatementListNodePtr WhileStatementNode::GetStatementList() {
  return m_stmt_list;
}

void WhileStatementNode::Accept(DesignExtractorPtr design_extractor) {
  WhileStatementNodePtr derived_ptr = std::dynamic_pointer_cast<WhileStatementNode>(shared_from_this());
  design_extractor->Visit(derived_ptr);
}

void WhileStatementNode::Accept(CfgBuilderPtr cfg_builder) {
  WhileStatementNodePtr derived_ptr = std::dynamic_pointer_cast<WhileStatementNode>(shared_from_this());
  cfg_builder->Visit(derived_ptr);
}

bool WhileStatementNode::operator==(const StatementNode &other) const {
  const auto casted_other = dynamic_cast<const WhileStatementNode *>(&other);
  StatementNodeStream this_stmt_list = m_stmt_list->GetStatements();
  StatementNodeStream other_stmt_list = casted_other->m_stmt_list->GetStatements();

  if (this_stmt_list.size() != other_stmt_list.size()) {
    return false;
  }

  for (int i = 0; i < this_stmt_list.size(); i++) {
    if (!(*(this_stmt_list.at(i)) == *(other_stmt_list.at(i)))) {
      return false;
    }
  }

  return m_stmt_no == casted_other->m_stmt_no && *m_condition == *(casted_other->m_condition);
}

}
