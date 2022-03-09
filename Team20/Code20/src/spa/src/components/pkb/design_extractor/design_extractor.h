#ifndef DESIGN_EXTRACTOR_H
#define DESIGN_EXTRACTOR_H

#include <unordered_set>

#include "populator.h"
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
#include "../../source_subsystem/ast/node_boolean_expression.h"
#include "../../source_subsystem/ast/node_not_expression.h"
#include "../../source_subsystem/ast/node_relational_expression.h"
#include "../../query_subsystem/pql_lexer/pql_token.h"

class DesignExtractor {
 private:
  ProgramNode root_node;
  Populator populator;

  void ProcNodeHandler(DesignExtractor *de, std::vector<std::string> visited, std::shared_ptr<ProcedureNode> proc, std::shared_ptr<StatementListNode> stmtLst);
  void ExprNodeHandler(std::vector<std::string> visited, std::shared_ptr<ProcedureNode> proc, std::string stmt, std::shared_ptr<ExpressionNode> expr);
  std::string ExprNodeHandler(std::vector<std::string> visited, std::shared_ptr<ProcedureNode> proc, std::string stmt, std::shared_ptr<ExpressionNode> expr, int direction, std::string pattern);
  void CondExprNodeHandler(std::vector<std::string> visited, std::shared_ptr<ProcedureNode> proc, std::string stmt, std::shared_ptr<ConditionalExpressionNode> if_stmt_cond);

 public:
  DesignExtractor(ProgramNode root_node, Populator populator);
  void TraverseAst();
//  void PopulateStmt(std::string stmt);
};

#endif //DESIGN_EXTRACTOR_H
