#include "design_extractor.h"
#include "../src/components/pkb/usage_store.h"

void DesignExtractor::TraverseAst() {
  // Todo

}
DesignExtractor::DesignExtractor(Program root_node, UsageStore storage) :
                                  root_node(root_node), storage(storage) {}

