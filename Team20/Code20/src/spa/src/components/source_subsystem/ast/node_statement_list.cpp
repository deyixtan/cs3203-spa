#include "node_statement_list.h"

StatementListNode::StatementListNode() : m_statements(std::vector<std::shared_ptr<StatementNode >>()) {}

StatementListNode::StatementListNode(std::vector<std::shared_ptr<StatementNode>> statements)
    : m_statements(statements) {}

std::vector<std::shared_ptr<StatementNode>> StatementListNode::GetStatements() {
  return m_statements;
}

std::string StatementListNode::ToString() {
  std::string str = "";
  for (std::shared_ptr<StatementNode> statement : m_statements) {
    str += statement->ToString();
  }
  return str;
}

std::string StatementListNode::Process(Populator populator, std::vector<std::string> *visited, std::shared_ptr<source::CfgGroupNode> cfg_node) {
  std::vector<std::shared_ptr<StatementNode>> stmts = m_statements;

  for (int i = 0; i < stmts.size(); ++i) {
    std::shared_ptr<StatementNode> stmt = stmts[i];
    std::string stmt_num = std::to_string(stmt->GetStatementNumber());
    std::string var_name = "";

    if (stmt != stmts.back()) {
      int curr_stmt = stmt->GetStatementNumber();
      int next_stmt = stmts[i + 1]->GetStatementNumber();
      populator.PopulateFollows(std::to_string(curr_stmt), std::to_string(next_stmt));

      // PopulateFollowsStar
      int curr_stmt_star = stmts[i]->GetStatementNumber();

      for (int j = i + 1; j < stmts.size(); ++j) {
        int next_stmt_star = stmts[j]->GetStatementNumber();
        populator.PopulateFollowsStar(std::to_string(curr_stmt_star), std::to_string(next_stmt_star));
      }
    }

    stmt->Process(populator, visited, cfg_node);
  }
  return "";
}

std::string StatementListNode::GetPatternFormat() {
  return "";
}

bool StatementListNode::operator==(const StatementListNode &other) const {
  if (m_statements.size() != other.m_statements.size()) {
    return false;
  }
  for (int i = 0; i < m_statements.size(); i++) {
    if (*(m_statements.at(i)) == *(other.m_statements.at(i))) {}
    else { return false; }
  }
  return true;
}
