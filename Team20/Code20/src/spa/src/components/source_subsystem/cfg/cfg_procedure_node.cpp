#include "cfg_procedure_node.h"

namespace source {

CfgProcedureNode::CfgProcedureNode() : m_nodes(std::vector<std::shared_ptr<CfgGroupNode>>()) {}

std::shared_ptr<CfgGroupNode> CfgProcedureNode::GetLastNode() {
  if (m_nodes.size() == 0) {
    throw std::runtime_error("Unable to get last node.");
  }
  return m_nodes.back();
}

void CfgProcedureNode::AddNode(std::shared_ptr<CfgGroupNode> cfg_node) {
  m_nodes.emplace_back(cfg_node);
}

}
