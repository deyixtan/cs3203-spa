#include "cfg_node.h"

namespace source {

CfgNode::CfgNode() = default;

CfgNodeStatementStream CfgNode::GetStatementList() {
  return m_stmt_list;
}

CfgNodeStream CfgNode::GetNextList() {
  return m_next_list;
}

void CfgNode::AddStatement(StmtType type, String &stmt_no) {
  m_stmt_list.push_back({type, stmt_no});
}

void CfgNode::AddNext(std::shared_ptr<CfgNode> &cfg_node) {
  m_next_list.push_back(cfg_node);
}

}
