#ifndef DESIGN_EXTRACTOR_H
#define DESIGN_EXTRACTOR_H

#include "../src/components/source_subsystem/node.h"
#include "../src/components/query_subsystem/pql_lexer/pql_token.h"
#include "../src/components/pkb/usage_store.h"
#include "../src/components/pkb/pkb.h"
#include <unordered_set>


class DesignExtractor {
 private:
  Program root_node;
  PKB *pkb;
  UsageStore storage;
  void traverse_ast();
  void process_proc(std::shared_ptr<Procedure> proc, std::shared_ptr<StatementList> stmtLst, std::vector<std::shared_ptr<Statement>> stmts);

  void populate_uses(int stmt, std::string var);
  void populate_modifies(int stmt, std::string var);
  void populate_proc(std::string name);
  void populate_assign(int stmt);
  void populate_stmt(int stmt);
  void populate_read(int stmt);
  void populate_print(int stmt);
  void populate_vars(std::string var);
  void populate_while(int stmt);
  void populate_if(int stmt);
  void populate_const(std::string name);
 public:
  DesignExtractor(Program root_node, PKB *pkb, UsageStore storage);

};

#endif //DESIGN_EXTRACTOR_H
