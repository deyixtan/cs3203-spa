#include "node_while_statement.h"
#include "../../iterator/design_extractor.h"
#include "../../iterator/cfg_builder.h"
#include "../cfg/cfg_node.h"

namespace source {

WhileStatementNode::WhileStatementNode(int stmt_no,
                                       std::shared_ptr<ConditionalExpressionNode> cond,
                                       std::shared_ptr<StatementListNode> stmt_list)
    : StatementNode(stmt_no), m_condition(cond), m_stmt_list(stmt_list) {}

std::shared_ptr<ConditionalExpressionNode> WhileStatementNode::GetCondition() {
  return m_condition;
}

std::shared_ptr<StatementListNode> WhileStatementNode::GetStatementList() {
  return m_stmt_list;
}

StmtType WhileStatementNode::GetStatementType() {
  return StmtType::WHILE;
}

std::string WhileStatementNode::ToString() {
  std::string str = StatementNode::ToString();
  return str + "while (" + m_condition->ToString() + ") {\n" + m_stmt_list->ToString() + str + "}\n";
}

std::string WhileStatementNode::GetPatternFormat() {
  return "";
}

bool WhileStatementNode::operator==(const StatementNode &other) const {
  const auto casted_other = dynamic_cast<const WhileStatementNode *>(&other);
  std::vector<std::shared_ptr<StatementNode>> this_stmt_list = m_stmt_list->GetStatements();
  std::vector<std::shared_ptr<StatementNode>> other_stmt_list = casted_other->m_stmt_list->GetStatements();

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

void WhileStatementNode::Accept(DesignExtractor *de, std::string proc_name) {
  std::string stmt_num = std::to_string(GetStatementNumber());
  de->GetPkbClient()->PopulateTypeOfStmt(stmt_num, WHILE);

  std::shared_ptr<StatementListNode> while_block = GetStatementList();
  std::vector<std::shared_ptr<StatementNode>> while_stmts = while_block->GetStatements();

  de->GetVisited().push_back(stmt_num);
  std::string cond_expr = de->Visit(m_condition, proc_name, true);

  de->Visit(while_block, proc_name);
  de->GetPkbClient()->PopulateWhile(de->GetVisited(), stmt_num, cond_expr);
}

std::shared_ptr<CfgNode> WhileStatementNode::Accept(CfgBuilder *cb, std::shared_ptr<CfgNode> cfg_node) {
  std::shared_ptr<CfgNode> body_node = std::make_shared<CfgNode>();
  std::shared_ptr<CfgNode> next_node = std::make_shared<CfgNode>();
  cfg_node->AddStatement(StmtType::WHILE, std::to_string(GetStatementNumber()));
  cfg_node->AddNext(body_node);
  cfg_node->AddNext(next_node);
  body_node = cb->Visit(m_stmt_list, body_node);
  body_node->AddNext(cfg_node);
  return next_node;
}

}
