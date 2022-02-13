#include "node_if_statement.h"

IfStatementNode::IfStatementNode(int stmt_no,
                                 std::shared_ptr<ConditionalExpressionNode> condition,
                                 std::shared_ptr<StatementListNode> if_stmt_list,
                                 std::shared_ptr<StatementListNode> else_stmt_list)
    : StatementNode(stmt_no), m_condition(condition), m_if_stmt_list(if_stmt_list), m_else_stmt_list(else_stmt_list) {}

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

std::string IfStatementNode::ToString(int level) {
  std::string str = StatementNode::ToString(level);
  return str + "if (" + m_condition->ToString(level) + ") then {\n" + m_if_stmt_list->ToString(level + 1) + str +
      "} else {\n" + m_else_stmt_list->ToString(level + 1) + str + "}\n";
}

bool IfStatementNode::operator==(const StatementNode &other) const {
  const auto casted_other = dynamic_cast<const IfStatementNode *>(&other);
  return m_stmt_no == casted_other->m_stmt_no && m_condition == casted_other->m_condition
      && m_if_stmt_list == casted_other->m_if_stmt_list && m_else_stmt_list == casted_other->m_else_stmt_list;
}
