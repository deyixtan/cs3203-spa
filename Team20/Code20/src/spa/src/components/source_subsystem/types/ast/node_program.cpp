#include "node_program.h"
#include "node_procedure.h"
#include "../cfg/cfg.h"
#include "../cfg/cfg_node.h"
#include "../../iterator/design_extractor.h"
#include "../../iterator/cfg_builder.h"

namespace source {

ProgramNode::ProgramNode() {}

ProgramNode::ProgramNode(std::vector<std::shared_ptr<ProcedureNode>> procedures) :
    m_procedures(procedures) {}

std::string ProgramNode::ToString() {
  std::string str = "";
  for (std::shared_ptr<ProcedureNode> procedure : m_procedures) {
    str += procedure->ToString() + "\n";
  }
  return str;
}

std::string ProgramNode::GetPatternFormat() {
  return "";
}

bool ProgramNode::operator==(const ProgramNode &other) const {
  if (m_procedures.size() != other.m_procedures.size()) {
    return false;
  }
  for (int i = 0; i < m_procedures.size(); i++) {
    if (*(m_procedures.at(i)) == *(other.m_procedures.at(i))) {}
    else { return false; }
  }
  return true;
}

void ProgramNode::Accept(DesignExtractor *de) {
  for (auto &procedure : m_procedures) {
    de->Visit(procedure);
  }
}

std::unordered_map<std::string, std::shared_ptr<CfgNode>> ProgramNode::Accept(CfgBuilder *cb) {
  std::unordered_map<std::string, std::shared_ptr<CfgNode>> heads;
  for (std::shared_ptr<ProcedureNode> const &procedure : m_procedures) {
    std::shared_ptr<CfgNode> head = std::make_shared<CfgNode>();
    std::shared_ptr<CfgNode> tail = cb->Visit(procedure, head);
    if (tail->GetStatementList().size() != 0) {
      std::shared_ptr<CfgNode> dummy = std::make_shared<CfgNode>();
      tail->AddNext(dummy);
    }
    heads.insert({procedure->GetIdentifier(), head});
  }
  return heads;
}

}
