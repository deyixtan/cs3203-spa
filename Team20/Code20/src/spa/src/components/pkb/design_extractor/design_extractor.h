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
#include "../../source_subsystem/ast/node_boolean_expression.h"
#include "../../source_subsystem/ast/node_not_expression.h"
#include "../../source_subsystem/ast/node_relational_expression.h"
#include "../../query_subsystem/pql_lexer/pql_token.h"

class DesignExtractor {
 private:
  ProgramNode root_node;
  PKB *pkb;
  UsageStore storage;

  void ProcNodeHandler(std::shared_ptr<ProcedureNode> proc,
                    std::shared_ptr<StatementListNode> stmtLst);
  void ExprNodeHandler(std::string stmt, std::shared_ptr<ExpressionNode> expr);
  std::string ExprNodeHandler(std::string stmt, std::shared_ptr<ExpressionNode> expr, int direction, std::string pattern);
  void CondExprNodeHandler(std::string stmt, std::shared_ptr<ConditionalExpressionNode> if_stmt_cond);

  void PopulateParent(std::string stmt1, std::string stmt2);
  void PopulateParentStar();
  void PopulateFollows(std::string stmt1, std::string stmt2);
  void PopulateFollowsStar(std::vector<std::shared_ptr<StatementNode>> stmts_lst, int index);
  void PopulateUses(std::string stmt, std::string var);
  void PopulateModifies(std::string stmt, std::string var);
  void PopulateProc(std::string name);
  void PopulateAssign(std::string stmt);
  void PopulateStmt(std::string stmt);
  void PopulateRead(std::string stmt);
  void PopulatePrint(std::string stmt);
  void PopulateVars(std::string var);
  void PopulateWhile(std::string stmt);
  void PopulateIf(std::string stmt);
  void PopulateConst(std::string name);
 public:
  DesignExtractor(ProgramNode root_node, PKB *pkb);
  void TraverseAst();
};

#endif //DESIGN_EXTRACTOR_H
