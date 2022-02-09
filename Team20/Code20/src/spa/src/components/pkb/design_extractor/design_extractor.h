#ifndef DESIGN_EXTRACTOR_H
#define DESIGN_EXTRACTOR_H

#include "../src/components/source_subsystem/node.h"
#include "../src/components/query_subsystem/pql_lexer/pql_token.h"
#include "../src/components/pkb/usage_store.h"
#include "../src/components/pkb/pkb.h"
#include <unordered_set>


class DesignExtractor {
 private:
  ProgramNode root_node;
  PKB *pkb;
  UsageStore storage;
  void process_proc(std::string proc_name, std::shared_ptr<StatementListNode> stmtLst, std::vector<std::shared_ptr<StatementNode>> stmts);

  void process_assign(std::string proc_name, std::string stmt_num, std::shared_ptr<ExpressionNode> expr);
  void populate_uses(std::string proc_name, std::string stmt, std::string var);
  void populate_modifies(std::string proc_name, std::string stmt, std::string var);
  void populate_proc(std::string name);
  void populate_assign(std::string stmt);
  void populate_stmt(std::string stmt);
  void populate_read(std::string stmt);
  void populate_print(std::string stmt);
  void populate_vars(std::string var);
  void populate_while(std::string stmt);
  void populate_if(std::string stmt);
  void populate_const(std::string name);
 public:
  DesignExtractor(ProgramNode root_node, PKB *pkb, UsageStore storage);
  void traverse_ast();
};

#endif //DESIGN_EXTRACTOR_H
