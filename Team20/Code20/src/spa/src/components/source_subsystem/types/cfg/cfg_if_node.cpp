#include "cfg_if_node.h"

namespace source {

CfgIfNode::CfgIfNode(CfgNode node, std::shared_ptr<CfgGroupNode> next, std::shared_ptr<CfgGroupNode> alt)
    : m_node(node), m_next(next), m_alt(alt) {}

CfgNode CfgIfNode::GetNode() {
  return m_node;
}

std::shared_ptr<CfgGroupNode> CfgIfNode::GetNext() {
  return m_next;
}

std::shared_ptr<CfgGroupNode> CfgIfNode::GetAlt() {
  return m_alt;
}

}
