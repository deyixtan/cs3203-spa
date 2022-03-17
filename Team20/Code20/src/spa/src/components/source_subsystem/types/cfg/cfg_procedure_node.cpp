#include "cfg_procedure_node.h"

namespace source {

CfgProcedureNode::CfgProcedureNode() : m_root() {}

std::shared_ptr<CfgGroupNode> CfgProcedureNode::GetRootNode() {
  if (m_root == nullptr) {
    m_root = std::make_shared<CfgGroupNode>();
    m_last = m_root;
  }
  return m_root;
}

std::shared_ptr<CfgGroupNode> CfgProcedureNode::GetLastNode() {
  if (m_last == nullptr) {
    m_last = std::make_shared<CfgGroupNode>();
    m_root = m_last;
  }
  return m_last;
}

void CfgProcedureNode::AddNode(std::shared_ptr<CfgGroupNode> cfg_node) {
  if (m_root == nullptr || m_last == nullptr) {
    m_root = cfg_node;
    m_last = cfg_node;
  }
  m_last->SetNext(cfg_node);
  m_last = cfg_node;
}

}
