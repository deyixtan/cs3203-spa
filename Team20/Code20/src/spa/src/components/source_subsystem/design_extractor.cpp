#include "design_extractor.h"
#include "components/pkb/pkb.h"

DesignExtractor::DesignExtractor(ProgramNode root_node, Populator populator) :
    root_node(root_node), populator(populator) {}

void DesignExtractor::TraverseAst() {
  std::vector<std::shared_ptr<ProcedureNode>> proc_list = this->root_node.GetProcedures();
  std::unordered_map<std::string, std::shared_ptr<source::CfgGroupNode>> procedure_map;

  for (auto &proc : proc_list) {
    std::vector<std::string> visited;
    source::CfgGroupNode cfg_root = source::CfgGroupNode();
    std::shared_ptr<source::CfgGroupNode> cfg_node_ptr = std::make_shared<source::CfgGroupNode>(cfg_root);
    proc->Process(populator, &visited, cfg_node_ptr);
    procedure_map.insert({proc->GetIdentifier(), cfg_node_ptr});
  }
  source::ProgramCfg program_cfg = source::ProgramCfg(procedure_map);
  populator.PopulateCfg(program_cfg);
}
