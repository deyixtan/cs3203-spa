#include "node_if_statement.h"
#include "components/source_subsystem/pkb_client.h"
#include "components/source_subsystem/iterator/cfg_builder.h"
#include "components/source_subsystem/iterator/design_extractor.h"
#include "components/source_subsystem/types/ast/node_conditional_expression.h"
#include "components/source_subsystem/types/ast/node_statement_list.h"

namespace source {

IfStatementNode::IfStatementNode(String &stmt_no,
                                 ConditionalExpressionNodePtr condition,
                                 StatementListNodePtr if_stmt_list,
                                 StatementListNodePtr else_stmt_list)
    : StatementNode(stmt_no),
      m_condition(std::move(condition)),
      m_if_stmt_list(std::move(if_stmt_list)),
      m_else_stmt_list(std::move(else_stmt_list)) {}

ConditionalExpressionNodePtr IfStatementNode::GetCondition() {
  return m_condition;
}

StatementListNodePtr IfStatementNode::GetIfStatementList() {
  return m_if_stmt_list;
}

StatementListNodePtr IfStatementNode::GetElseStatementList() {
  return m_else_stmt_list;
}

StatementNodeStream IfStatementNode::GetAllStatementList() {
  StatementNodeStream if_stmt_list = m_if_stmt_list->GetStatements();
  StatementNodeStream else_stmt_list = m_else_stmt_list->GetStatements();
  if_stmt_list.insert(end(if_stmt_list), begin(else_stmt_list), end(else_stmt_list));
  return if_stmt_list;
}

void IfStatementNode::Accept(DesignExtractorPtr design_extractor) {
  design_extractor->Visit(shared_from_this());
}

CfgNodePtr IfStatementNode::Accept(CfgBuilder *cfg_builder, CfgNodePtr cfg_node) {
//  CfgNodePtr condition_node = std::make_shared<CfgNode>();
//  cfg_node->AddNext(condition_node);
//  cfg_node = condition_node;

  CfgNodePtr if_node = std::make_shared<CfgNode>();
  CfgNodePtr else_node = std::make_shared<CfgNode>();
  CfgNodePtr next_node = std::make_shared<CfgNode>();
  cfg_node->AddStatement(StmtType::IF, GetStatementNumber());
  cfg_node->AddNext(if_node);
  cfg_node->AddNext(else_node);
  if_node = cfg_builder->Visit(m_if_stmt_list, if_node);
  if_node->AddNext(next_node);
  else_node = cfg_builder->Visit(m_else_stmt_list, else_node);
  else_node->AddNext(next_node);
  return next_node;
}

bool IfStatementNode::operator==(const StatementNode &other) const {
  const auto casted_other = dynamic_cast<const IfStatementNode *>(&other);
  StatementNodeStream if_this_stmt_list = m_if_stmt_list->GetStatements();
  StatementNodeStream if_other_stmt_list = casted_other->m_if_stmt_list->GetStatements();
  StatementNodeStream else_this_stmt_list = m_else_stmt_list->GetStatements();
  StatementNodeStream else_other_stmt_list = casted_other->m_else_stmt_list->GetStatements();

  if (if_this_stmt_list.size() != if_other_stmt_list.size()) {
    return false;
  }
  for (int i = 0; i < if_this_stmt_list.size(); i++) {
    if (*(if_this_stmt_list.at(i)) == *(if_other_stmt_list.at(i))) {}
    else { return false; }
  }

  if (else_this_stmt_list.size() != else_other_stmt_list.size()) {
    return false;
  }
  for (int i = 0; i < else_this_stmt_list.size(); i++) {
    if (*(else_this_stmt_list.at(i)) == *(else_other_stmt_list.at(i))) {}
    else { return false; }
  }

  return m_stmt_no == casted_other->m_stmt_no && *m_condition == *(casted_other->m_condition);
}

StmtType IfStatementNode::GetStatementType() {
  return StmtType::IF;
}

}
