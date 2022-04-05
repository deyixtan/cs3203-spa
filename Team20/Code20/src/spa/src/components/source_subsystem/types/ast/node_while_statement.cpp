#include "node_while_statement.h"
#include "../../iterator/design_extractor.h"
#include "../../iterator/cfg_builder.h"
#include "../cfg/cfg_node.h"
#include "node_conditional_expression.h"
#include "node_statement_list.h"

namespace source {

WhileStatementNode::WhileStatementNode(String stmt_no, ConditionalExpressionNodePtr condition, StatementListNodePtr stmt_list)
    : StatementNode(stmt_no), m_condition(condition), m_stmt_list(stmt_list) {}

ConditionalExpressionNodePtr WhileStatementNode::GetCondition() {
  return m_condition;
}

StatementListNodePtr WhileStatementNode::GetStatementList() {
  return m_stmt_list;
}

StmtType WhileStatementNode::GetStatementType() {
  return StmtType::WHILE;
}

String WhileStatementNode::GetPatternFormat() {
  return "";
}

void WhileStatementNode::Accept(DesignExtractor *de, String proc_name) {
  String stmt_num = GetStatementNumber();
  de->GetPkbClient()->PopulateTypeOfStmt(stmt_num, WHILE);

  StatementListNodePtr while_block = GetStatementList();
  StatementNodeStream while_stmts = while_block->GetStatements();

  de->GetVisited().push_back(stmt_num);
  String cond_expr = de->Visit(m_condition, proc_name, true);

  de->Visit(while_block, proc_name);
  de->GetPkbClient()->PopulateWhile(de->GetVisited(), stmt_num, cond_expr);
}

CfgNodePtr WhileStatementNode::Accept(CfgBuilder *cb, CfgNodePtr cfg_node) {
  CfgNodePtr body_node = std::make_shared<CfgNode>();
  CfgNodePtr next_node = std::make_shared<CfgNode>();
  cfg_node->AddStatement(StmtType::WHILE, GetStatementNumber());
  cfg_node->AddNext(body_node);
  cfg_node->AddNext(next_node);
  body_node = cb->Visit(m_stmt_list, body_node);
  body_node->AddNext(cfg_node);
  return next_node;
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
