#include "cfg_node.h"

CfgNode::CfgNode() {}

std::vector<std::string> CfgNode::GetStatementList() {
  return m_stmt_no_list;
}

std::vector<std::shared_ptr<CfgNode>> CfgNode::GetDescendants() {
  return m_next_list;
}

void CfgNode::AddStatement(std::string stmt_no) {
  m_stmt_no_list.push_back(stmt_no);
}

void CfgNode::AddNext(std::shared_ptr<CfgNode> cfg_node) {
  m_next_list.push_back(cfg_node);
}
