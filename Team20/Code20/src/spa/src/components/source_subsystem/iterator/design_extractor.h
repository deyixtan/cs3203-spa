#ifndef DESIGN_EXTRACTOR_H
#define DESIGN_EXTRACTOR_H

#include <unordered_set>

#include "components/pkb/pkb.h"
#include "components/source_subsystem/populator.h"
#include "components/source_subsystem/types/ast/node_program.h"
#include "components/source_subsystem/types/cfg/cfg_program_node.h"

class DesignExtractor {
 private:
  ProgramNode root_node;
  Populator populator;

 public:
  DesignExtractor(ProgramNode root_node, Populator populator);
  void TraverseAst();
};

#endif //DESIGN_EXTRACTOR_H
