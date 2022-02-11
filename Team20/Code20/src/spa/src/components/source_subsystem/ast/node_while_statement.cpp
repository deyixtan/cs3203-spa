#include "node_while_statement.h"

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

std::string WhileStatementNode::ToString(int level) {
  std::string str = StatementNode::ToString(level);
  return str + "while (" + m_condition->ToString(level) + ") {\n" + m_stmt_list->ToString(level + 1) + str + "}\n";
}
