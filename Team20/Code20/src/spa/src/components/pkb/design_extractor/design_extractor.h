#ifndef DESIGN_EXTRACTOR_H
#define DESIGN_EXTRACTOR_H

#include "../src/components/source_subsystem/node.h"
#include "../src/components/query_subsystem/pql_lexer/pql_token.h"
#include "../src/components/pkb/usage_store.h"
#include <unordered_set>


class DesignExtractor {
 private:
  AST ast;
  UsageStore storage;
  void TraverseAst();
 public:
  DesignExtractor();
  DesignExtractor(const AST ast, const UsageStore storage);

};

#endif //DESIGN_EXTRACTOR_H
