#include "node_statement_list.h"
#include "components/source_subsystem/pkb_client.h"
#include "../../iterator/design_extractor.h"
#include "../../iterator/cfg_builder.h"
#include "node_statement.h"
#include "../cfg/cfg_node.h"

namespace source {

StatementListNode::StatementListNode() : m_statements(StatementNodeStream()) {}

StatementListNode::StatementListNode(StatementNodeStream statements)
    : m_statements(std::move(statements)) {}

StatementNodeStream StatementListNode::GetStatements() {
  return m_statements;
}
void StatementListNode::Accept(DesignExtractor *de, String &proc_name) {
  StatementNodeStream stmts = m_statements;

  for (int i = 0; i < stmts.size(); ++i) {
    StatementNodePtr stmt = stmts[i];
    de->Visit(stmt, proc_name);
    String stmt_num = stmt->GetStatementNumber();
    if (!de->GetVisited().empty()) {
      de->GetPkbClient()->PopulateParent(de->GetVisited().back(), stmt_num);
    }

    if (i > 0) {
      String prev_stmt = stmts[i - 1]->GetStatementNumber();
      String curr_stmt = stmt->GetStatementNumber();
      de->GetPkbClient()->PopulateFollows(prev_stmt, curr_stmt);

      // PopulateFollowsStar
      for (int j = i - 1; j >= 0; --j) {
        prev_stmt = stmts[j]->GetStatementNumber();
        de->GetPkbClient()->PopulateFollowsStar(prev_stmt, curr_stmt);
      }
    }
  }
}

CfgNodePtr StatementListNode::Accept(CfgBuilder *cb, CfgNodePtr cfg_node) {
  for (StatementNodePtr &stmt : m_statements) {
    // TODO: to remove
    bool is_empty_cfg_node = cfg_node->GetStatementList().empty();
    if ((stmt->GetStatementType() == IF || stmt->GetStatementType() == WHILE) && !is_empty_cfg_node) {
      CfgNodePtr condition_node = std::make_shared<CfgNode>();
      cfg_node->AddNext(condition_node);
      cfg_node = condition_node;
    }
    cfg_node = cb->Visit(stmt, cfg_node);
  }
  return cfg_node;
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

}
