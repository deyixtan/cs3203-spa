#ifndef DESIGN_EXTRACTOR_H
#define DESIGN_EXTRACTOR_H

#include <unordered_set>

#include "../pkb.h"
#include "../usage_store.h"
#include "../../source_subsystem/ast/node_program.h"
#include "../../source_subsystem/ast/node_read_statement.h"
#include "../../source_subsystem/ast/node_print_statement.h"
#include "../../source_subsystem/ast/node_while_statement.h"
#include "../../source_subsystem/ast/node_if_statement.h"
#include "../../source_subsystem/ast/node_assign_statement.h"
#include "../../source_subsystem/ast/node_combination_expression.h"
#include "../../source_subsystem/ast/node_constant.h"
#include "../../query_subsystem/pql_lexer/pql_token.h"

class DesignExtractor {
 private:
  ProgramNode root_node;
  PKB *pkb;
  UsageStore storage;
  void process_proc(std::shared_ptr<ProcedureNode> proc,
                    std::shared_ptr<StatementListNode> stmtLst,
                    std::vector<std::shared_ptr<StatementNode>> stmts);
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
  DesignExtractor(ProgramNode root_node, PKB *pkb, UsageStore storage);
  void traverse_ast();
};

#endif //DESIGN_EXTRACTOR_H
