#include "node_program.h"
#include "node_procedure.h"
#include "../../iterator/design_extractor.h"

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

std::string ProgramNode::Process(Populator populator, std::vector<std::string> *visited, bool is_uses, std::shared_ptr<source::CfgProcedureNode> cfg_proc_node, std::shared_ptr<source::CfgGroupNode> cfg_node) {
  std::unordered_map<std::string, std::shared_ptr<source::CfgProcedureNode>> procedure_map;

  for (auto &procedure : m_procedures) {
    source::CfgGroupNode cfg_root = source::CfgGroupNode();
    std::shared_ptr<source::CfgProcedureNode> cfg_proc_node_ptr = std::make_shared<source::CfgProcedureNode>();
    procedure->Process(populator, visited, false, cfg_proc_node_ptr, nullptr);
    procedure_map.insert({procedure->GetIdentifier(), cfg_proc_node_ptr});
  }

  source::CfgProgramNode program_cfg = source::CfgProgramNode(procedure_map);
  populator.PopulateCfg(program_cfg);
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