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

  void StoreProc(ProcedureNode procedure_node);

  std::unordered_set<std::string> proc_list;
  std::unordered_set<int> stmt_list;
  std::unordered_set<int> while_stmt_list;
  std::unordered_set<int> read_stmt_list;
  std::unordered_set<int> print_stmt_list;
  std::unordered_set<int> call_stmt_list;
  std::unordered_set<int> if_stmt_list; //need to create separate else-then or treat this as a block?
  std::unordered_set<int> assign_stmt_list;

  std::unordered_set<std::string> var_list;
  std::unordered_set<std::string> const_list;

};

#endif //DESIGN_EXTRACTOR_H
