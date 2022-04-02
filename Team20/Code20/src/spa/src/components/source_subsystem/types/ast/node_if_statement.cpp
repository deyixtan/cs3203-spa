#include "node_if_statement.h"
#include "../../iterator/design_extractor.h"
#include "../../iterator/cfg_builder.h"
#include "../cfg/cfg_node.h"

IfStatementNode::IfStatementNode(int stmt_no,
                                 std::shared_ptr<ConditionalExpressionNode> condition,
                                 std::shared_ptr<StatementListNode> if_stmt_list,
                                 std::shared_ptr<StatementListNode> else_stmt_list)
    : StatementNode(stmt_no),
    m_condition(condition),
    m_if_stmt_list(if_stmt_list),
    m_else_stmt_list(else_stmt_list) {}

std::shared_ptr<ConditionalExpressionNode> IfStatementNode::GetCondition() {
  return m_condition;
}

std::shared_ptr<StatementListNode> IfStatementNode::GetIfStatementList() {
  return m_if_stmt_list;
}

std::shared_ptr<StatementListNode> IfStatementNode::GetElseStatementList() {
  return m_else_stmt_list;
}

std::vector<std::shared_ptr<StatementNode>> IfStatementNode::GetAllStatementList() {
  std::vector<std::shared_ptr<StatementNode>> if_stmt_list = m_if_stmt_list->GetStatements();
  std::vector<std::shared_ptr<StatementNode>> else_stmt_list = m_else_stmt_list->GetStatements();
  if_stmt_list.insert(end(if_stmt_list), begin(else_stmt_list), end(else_stmt_list));
  return if_stmt_list;
}

StmtType IfStatementNode::GetStatementType() {
  return StmtType::IF;
}

std::string IfStatementNode::ToString() {
  std::string str = StatementNode::ToString();
  return str + "if (" + m_condition->ToString() + ") then {\n" + m_if_stmt_list->ToString() + str +
      "} else {\n" + m_else_stmt_list->ToString() + str + "}\n";
}

std::string IfStatementNode::GetPatternFormat() {
  return "";
}

bool IfStatementNode::operator==(const StatementNode &other) const {
  const auto casted_other = dynamic_cast<const IfStatementNode *>(&other);
  std::vector<std::shared_ptr<StatementNode>> if_this_stmt_list = m_if_stmt_list->GetStatements();
  std::vector<std::shared_ptr<StatementNode>> if_other_stmt_list = casted_other->m_if_stmt_list->GetStatements();
  std::vector<std::shared_ptr<StatementNode>> else_this_stmt_list = m_else_stmt_list->GetStatements();
  std::vector<std::shared_ptr<StatementNode>> else_other_stmt_list = casted_other->m_else_stmt_list->GetStatements();

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

void IfStatementNode::Accept(DesignExtractor *de, std::string proc_name) {
  std::string stmt_num = std::to_string(GetStatementNumber());
  std::shared_ptr<StatementListNode> if_block = m_if_stmt_list;
  std::shared_ptr<StatementListNode> else_block = m_else_stmt_list;
  std::vector<std::shared_ptr<StatementNode>> if_stmts = m_if_stmt_list->GetStatements();
  std::vector<std::shared_ptr<StatementNode>> else_stmts =  m_else_stmt_list->GetStatements();

  de->GetVisited().push_back(stmt_num);

  std::string cond_expr = de->Visit(m_condition, proc_name, true);
  de->Visit(if_block, proc_name);
  de->Visit(else_block, proc_name);

  de->GetPkbClient()->PopulateIf(de->GetVisited(), stmt_num, cond_expr);
  de->GetPkbClient()->PopulateTypeOfStmt(stmt_num, IF);
}

std::shared_ptr<CfgNode> IfStatementNode::Accept(CfgBuilder *cb, std::shared_ptr<CfgNode> cfg_node) {
  std::shared_ptr<CfgNode> if_node = std::make_shared<CfgNode>();
  std::shared_ptr<CfgNode> else_node = std::make_shared<CfgNode>();
  std::shared_ptr<CfgNode> next_node = std::make_shared<CfgNode>();
  cfg_node->AddStatement(StmtType::IF, std::to_string(GetStatementNumber()));
  cfg_node->AddNext(if_node);
  cfg_node->AddNext(else_node);
  if_node = cb->Visit(m_if_stmt_list, if_node);
  if_node->AddNext(next_node);
  else_node = cb->Visit(m_else_stmt_list, else_node);
  else_node->AddNext(next_node);
  return next_node;
}
