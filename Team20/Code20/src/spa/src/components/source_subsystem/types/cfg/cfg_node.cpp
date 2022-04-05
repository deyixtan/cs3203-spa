#include "cfg_node.h"

namespace source {

CfgNode::CfgNode() {}

std::vector<Statement> CfgNode::GetStatementList() {
  return m_stmt_no_list;
}

std::vector<std::shared_ptr<CfgNode>> CfgNode::GetDescendants() {
  return m_next_list;
}

void CfgNode::AddStatement(StmtType type, std::string stmt_no) {
  m_stmt_no_list.push_back({type, stmt_no});
}

void CfgNode::AddNext(std::shared_ptr<CfgNode> cfg_node) {
  m_next_list.push_back(cfg_node);
}

}
