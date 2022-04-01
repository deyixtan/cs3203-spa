#include "node_statement_list.h"
#include "../../iterator/design_extractor.h"
#include "../../iterator/cfg_builder.h"
#include "node_statement.h"
#include "../cfg/cfg_node.h"

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

void StatementListNode::Accept(DesignExtractor *de, std::string proc_name) {
  std::vector<std::shared_ptr<StatementNode>> stmts = m_statements;

  for (int i = 0; i < stmts.size(); ++i) {
    std::shared_ptr<StatementNode> stmt = stmts[i];
    de->Visit(stmt, proc_name);
    std::string stmt_num = std::to_string(stmt->GetStatementNumber());
    std::string var_name = "";
    if (de->GetVisited().size() > 0) {
      de->GetPkbClient()->PopulateParent(de->GetVisited().back(), stmt_num);
    }

    if (i > 0) {
      int prev_stmt = stmts[i - 1]->GetStatementNumber();
      int curr_stmt = stmt->GetStatementNumber();
      de->GetPkbClient()->PopulateFollows(std::to_string(prev_stmt), std::to_string(curr_stmt));

      // PopulateFollowsStar
      for (int j = i - 1; j >= 0; --j) {
        prev_stmt = stmts[j]->GetStatementNumber();
        de->GetPkbClient()->PopulateFollowsStar(std::to_string(prev_stmt), std::to_string(curr_stmt));
      }
    }
  }
}

std::shared_ptr<CfgNode> StatementListNode::Accept(CfgBuilder *cb, std::shared_ptr<CfgNode> cfg_node) {
  for (std::shared_ptr<StatementNode> stmt : m_statements) {
    bool is_empty_cfg_node = cfg_node->GetStatementList().size() == 0;
    if ((stmt->GetStatementType() == IF || stmt->GetStatementType() == WHILE) && !is_empty_cfg_node) {
        std::shared_ptr<CfgNode> condition_node = std::make_shared<CfgNode>();
        cfg_node->AddNext(condition_node);
        cfg_node = condition_node;
    }
    cfg_node = cb->Visit(stmt, cfg_node);
  }
  return cfg_node;
}
