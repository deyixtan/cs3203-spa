#include "design_extractor.h"
#include "components/pkb/pkb.h"

DesignExtractor::DesignExtractor(ProgramNode root_node, Populator populator) :
    root_node(root_node), populator(populator) {}

void DesignExtractor::TraverseAst() {
  std::vector<std::string> visited;
  root_node.Process(populator, &visited, false, nullptr, nullptr);
}
