#include "design_extractor.h"
#include "../src/components/pkb/usage_store.h"

void DesignExtractor::TraverseAst() {
  // Todo

}


DesignExtractor::DesignExtractor() {}
DesignExtractor::DesignExtractor(const AST ast, const UsageStore storage) {
  this->ast = ast;
  this->storage = storage;
}


