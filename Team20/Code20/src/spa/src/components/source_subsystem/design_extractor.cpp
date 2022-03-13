#include "design_extractor.h"
#include "components/pkb/pkb.h"

DesignExtractor::DesignExtractor(ProgramNode root_node, Populator populator) :
    root_node(root_node), populator(populator) {}

void DesignExtractor::TraverseAst() {
  std::vector<std::shared_ptr<ProcedureNode>> proc_list = this->root_node.GetProcedures();
  for (auto &proc : proc_list) {
    std::vector<std::string> visited;
    proc->Process(populator, &visited);
  }
}
