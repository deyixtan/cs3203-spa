#include "design_extractor.h"
#include "components/pkb/pkb.h"

DesignExtractor::DesignExtractor(ProgramNode root_node, Populator populator) :
    root_node(root_node), populator(populator) {}

void DesignExtractor::TraverseAst() {
  std::vector<std::shared_ptr<ProcedureNode>> proc_list = this->root_node.GetProcedures();
  std::unordered_map<std::string, std::shared_ptr<source::CfgProcedureNode>> procedure_map;

  for (auto &proc : proc_list) {
    std::vector<std::string> visited;
    source::CfgGroupNode cfg_root = source::CfgGroupNode();
    std::shared_ptr<source::CfgProcedureNode> cfg_proc_node_ptr = std::make_shared<source::CfgProcedureNode>();
    proc->Process(populator, &visited, false, cfg_proc_node_ptr, nullptr, proc->GetIdentifier());
    procedure_map.insert({proc->GetIdentifier(), cfg_proc_node_ptr});
  }

  source::CfgProgramNode program_cfg = source::CfgProgramNode(procedure_map);
  populator.PopulateCfg(program_cfg);
}
