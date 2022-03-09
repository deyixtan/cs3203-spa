#include "node_if_statement.h"

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

std::string IfStatementNode::ToString(int level) {
  std::string str = StatementNode::ToString(level);
  return str + "if (" + m_condition->ToString(level) + ") then {\n" + m_if_stmt_list->ToString(level + 1) + str +
      "} else {\n" + m_else_stmt_list->ToString(level + 1) + str + "}\n";
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

void IfStatementNode::Process(Populator populator, std::vector<std::string>* visited) {
  std::string stmt_num = std::to_string(GetStatementNumber());
  populator.PopulateStmt(stmt_num);
  std::string if_stmt_num = std::to_string(GetStatementNumber());
  visited->push_back(if_stmt_num);

  m_condition->Process(populator, visited);
  std::vector<std::shared_ptr<StatementNode>> if_stmts = m_if_stmt_list->GetStatements();
  std::vector<std::shared_ptr<StatementNode>> else_stmts =  m_else_stmt_list->GetStatements();

  for (int j = 0; j < if_stmts.size(); ++j) {
    int curr = if_stmts[j]->GetStatementNumber();
    populator.PopulateParent(stmt_num, std::to_string(curr));
  }

  for (int j = 0; j < else_stmts.size(); ++j) {
    int curr = else_stmts[j]->GetStatementNumber();
    populator.PopulateParent(stmt_num, std::to_string(curr));
  }
  populator.PopulateIf(stmt_num);
  populator.PopulateParentStar(if_stmt_num, *visited);
  visited->pop_back();
  populator.PopulateParentStar(stmt_num, *visited);
}