#include "cfg_group_node.h"

namespace source {

CfgGroupNode::CfgGroupNode() : CfgGroupNode(std::vector<CfgNode>()) {}

CfgGroupNode::CfgGroupNode(std::vector<CfgNode> nodes) : CfgGroupNode(nodes, nullptr) {}

CfgGroupNode::CfgGroupNode(std::vector<CfgNode> nodes, std::shared_ptr<CfgGroupNode> next)
    : m_nodes(nodes), m_next(next) {}

std::vector<CfgNode> CfgGroupNode::GetNodes() {
  return m_nodes;
}

std::shared_ptr<CfgGroupNode> CfgGroupNode::GetNext() {
  return m_next;
}

}
