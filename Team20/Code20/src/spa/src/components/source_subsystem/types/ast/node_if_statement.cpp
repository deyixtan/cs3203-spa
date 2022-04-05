#include "node_if_statement.h"
#include "../../iterator/design_extractor.h"
#include "../../iterator/cfg_builder.h"
#include "../cfg/cfg_node.h"
#include "node_conditional_expression.h"
#include "node_statement_list.h"
#include "components/source_subsystem/pkb_client.h"

namespace source {

IfStatementNode::IfStatementNode(String stmt_no,
                                 ConditionalExpressionNodePtr condition,
                                 StatementListNodePtr if_stmt_list,
                                 StatementListNodePtr else_stmt_list)
    : StatementNode(stmt_no),
      m_condition(condition),
      m_if_stmt_list(if_stmt_list),
      m_else_stmt_list(else_stmt_list) {}

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

StmtType IfStatementNode::GetStatementType() {
  return StmtType::IF;
}

String IfStatementNode::GetPatternFormat() {
  return "";
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

void IfStatementNode::Accept(DesignExtractor *de, String proc_name) {
  String stmt_num = GetStatementNumber();
  de->GetPkbClient()->PopulateTypeOfStmt(stmt_num, IF);

  StatementListNodePtr if_block = m_if_stmt_list;
  StatementListNodePtr else_block = m_else_stmt_list;
  StatementNodeStream if_stmts = m_if_stmt_list->GetStatements();
  StatementNodeStream else_stmts = m_else_stmt_list->GetStatements();

  de->GetVisited().push_back(stmt_num);

  String cond_expr = de->Visit(m_condition, proc_name, true);
  de->Visit(if_block, proc_name);
  de->Visit(else_block, proc_name);

  de->GetPkbClient()->PopulateIf(de->GetVisited(), stmt_num, cond_expr);
}

CfgNodePtr IfStatementNode::Accept(CfgBuilder *cb, CfgNodePtr cfg_node) {
  CfgNodePtr if_node = std::make_shared<CfgNode>();
  CfgNodePtr else_node = std::make_shared<CfgNode>();
  CfgNodePtr next_node = std::make_shared<CfgNode>();
  cfg_node->AddStatement(StmtType::IF, GetStatementNumber());
  cfg_node->AddNext(if_node);
  cfg_node->AddNext(else_node);
  if_node = cb->Visit(m_if_stmt_list, if_node);
  if_node->AddNext(next_node);
  else_node = cb->Visit(m_else_stmt_list, else_node);
  else_node->AddNext(next_node);
  return next_node;
}

}
