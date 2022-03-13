#ifndef DESIGN_EXTRACTOR_H
#define DESIGN_EXTRACTOR_H

#include <unordered_set>

#include "components/pkb/pkb.h"
#include "populator.h"
#include "components/source_subsystem/ast/node_program.h"
#include "cfg/cfg.h"

class DesignExtractor {
 private:
  ProgramNode root_node;
  Populator populator;

 public:
  DesignExtractor(ProgramNode root_node, Populator populator);
  void TraverseAst();
};

#endif //DESIGN_EXTRACTOR_H
