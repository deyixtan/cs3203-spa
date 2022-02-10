#include "node_while_statement.h"

WhileStatementNode::WhileStatementNode(int line,
                                       std::shared_ptr<ConditionalExpressionNode> cond,
                                       std::shared_ptr<StatementListNode> stmt_list)
    : StatementNode(line), m_condition(cond), m_stmt_list(stmt_list) {}

std::vector<std::shared_ptr<StatementNode>> WhileStatementNode::getStatementList() {
  return m_stmt_list->getStatements();
}

std::shared_ptr<ConditionalExpressionNode> WhileStatementNode::getConditional() {
  return m_condition;
}

std::shared_ptr<StatementListNode> WhileStatementNode::getBody() {
  return m_stmt_list;
}

StmtType WhileStatementNode::getStatementType() {
  return StmtType::WHILE;
}

std::string WhileStatementNode::format(int level) {
  std::string header = StatementNode::format(level);
  return header + "while (" + m_condition->format(level) + ") {\n" + m_stmt_list->format(level + 1) + header + "}\n";
}
