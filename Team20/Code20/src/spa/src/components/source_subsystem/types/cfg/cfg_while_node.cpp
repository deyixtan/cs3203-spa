#include "cfg_while_node.h"

namespace source {

CfgWhileNode::CfgWhileNode(CfgNode node, std::shared_ptr<CfgGroupNode> body, std::shared_ptr<CfgGroupNode> next)
    : m_node(node), m_body(body), m_next(next) {}

CfgNode CfgWhileNode::GetNode() {
  return m_node;
}

std::shared_ptr<CfgGroupNode> CfgWhileNode::GetBody() {
  return m_body;
}

std::shared_ptr<CfgGroupNode> CfgWhileNode::GetNext() {
  return m_next;
}

}
