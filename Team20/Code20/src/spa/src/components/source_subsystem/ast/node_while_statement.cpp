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

bool WhileStatementNode::operator==(const StatementNode &other) const {
  const auto casted_other = dynamic_cast<const WhileStatementNode *>(&other);
  std::vector<std::shared_ptr<StatementNode>> this_stmt_list = m_stmt_list->GetStatements();
  std::vector<std::shared_ptr<StatementNode>> other_stmt_list = casted_other->m_stmt_list->GetStatements();

  if (this_stmt_list.size() != other_stmt_list.size()) {
    return false;
  }
  for (int i = 0; i < this_stmt_list.size(); i++) {
    if (*(this_stmt_list.at(i)) == *(other_stmt_list.at(i))) {}
    else { return false; }
  }

  return m_stmt_no == casted_other->m_stmt_no && *m_condition == *(casted_other->m_condition);
}

void WhileStatementNode::Process(Populator populator, std::vector<std::string>* visited) {
  std::string stmt_num = std::to_string(GetStatementNumber());
  populator.PopulateStmt(stmt_num);
  std::string while_stmt_num = std::to_string(GetStatementNumber());
  visited->push_back(while_stmt_num);

  m_condition->Process(populator, visited, stmt_num);

  std::shared_ptr<StatementListNode> while_block = GetStatementList();
  std::vector<std::shared_ptr<StatementNode>> while_stmts = while_block->GetStatements();

  for (int j = 0; j < while_stmts.size(); ++j) {
    int curr = while_stmts[j]->GetStatementNumber();
    populator.PopulateParent(stmt_num, std::to_string(curr));
  }
  populator.PopulateWhile(stmt_num);
  populator.PopulateParentStar(while_stmt_num, *visited);
  visited->pop_back();
  populator.PopulateParentStar(stmt_num, *visited);
}
