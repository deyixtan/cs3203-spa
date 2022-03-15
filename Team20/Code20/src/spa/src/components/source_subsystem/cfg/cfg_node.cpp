#include "cfg_node.h"

namespace source {

CfgNode::CfgNode(int stmt_no) : m_stmt_no(stmt_no) {}

int CfgNode::GetStatementNumber() {
  return m_stmt_no;
}

};
