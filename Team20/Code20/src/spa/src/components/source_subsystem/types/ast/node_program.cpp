#include "node_program.h"
#include "node_procedure.h"
#include "../cfg/cfg.h"
#include "../cfg/cfg_node.h"
#include "../../iterator/design_extractor.h"
#include "../../iterator/cfg_builder.h"

ProgramNode::ProgramNode() {}

ProgramNode::ProgramNode(std::vector<std::shared_ptr<ProcedureNode>> procedures) :
    m_procedures(procedures) {}

std::vector<std::shared_ptr<ProcedureNode>> ProgramNode::GetProcedures() {
  return m_procedures;
}

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

void ProgramNode::Accept(CfgBuilder *cb) {
  std::unordered_map<std::string, std::shared_ptr<CfgNode>> heads;
  for (std::shared_ptr<ProcedureNode> &procedure : m_procedures) {
    std::shared_ptr<CfgNode> node = std::make_shared<CfgNode>();
    std::shared_ptr<CfgNode> dummy_node = cb->Visit(procedure, node);
    std::shared_ptr<CfgNode> endProcBlock = std::make_shared<CfgNode>();
    dummy_node->AddNext(endProcBlock);
    endProcBlock->AddStatement("");
    heads.insert({procedure->GetIdentifier(), node});
  }
  std::shared_ptr<Cfg> program_cfg = std::make_shared<Cfg>(heads);
  cb->GetPkbClient()->PopulateCfg(*program_cfg);
}
