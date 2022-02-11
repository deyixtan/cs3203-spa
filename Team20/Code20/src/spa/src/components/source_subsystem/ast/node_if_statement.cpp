#include "node_if_statement.h"

IfStatementNode::IfStatementNode(int line,
                                 std::shared_ptr<ConditionalExpressionNode> condition,
                                 std::shared_ptr<StatementListNode> if_stmt_list,
                                 std::shared_ptr<StatementListNode> else_stmt_list)
    : StatementNode(line), m_condition(condition), m_if_stmt_list(if_stmt_list), m_else_stmt_list(else_stmt_list) {}

std::vector<std::shared_ptr<StatementNode>> IfStatementNode::getStatementList() {
  std::vector<std::shared_ptr<StatementNode>> consequentStatements = m_if_stmt_list->getStatements();
  std::vector<std::shared_ptr<StatementNode>> alternativeStatements = m_else_stmt_list->getStatements();
  consequentStatements.insert(end(consequentStatements), begin(alternativeStatements), end(alternativeStatements));
  return consequentStatements;
}

std::shared_ptr<ConditionalExpressionNode> IfStatementNode::getConditional() {
  return m_condition;
}

std::shared_ptr<StatementListNode> IfStatementNode::getConsequent() {
  return m_if_stmt_list;
}

std::shared_ptr<StatementListNode> IfStatementNode::getAlternative() {
  return m_else_stmt_list;
}

StmtType IfStatementNode::getStatementType() {
  return StmtType::IF;
}

std::string IfStatementNode::ToString(int level) {
  std::string header = StatementNode::ToString(level);
  return header + "if (" + m_condition->ToString(level) + ") then {\n" + m_if_stmt_list->ToString(level + 1) + header +
      "} else {\n" + m_else_stmt_list->ToString(level + 1) + header + "}\n";
}
