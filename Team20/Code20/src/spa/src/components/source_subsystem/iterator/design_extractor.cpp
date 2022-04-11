#include "design_extractor.h"
#include "components/pkb/pkb_client/pkb_client.h"
#include "components/source_subsystem/types/ast/node_assign_statement.h"
#include "components/source_subsystem/types/ast/node_boolean_expression.h"
#include "components/source_subsystem/types/ast/node_call_statement.h"
#include "components/source_subsystem/types/ast/node_combination_expression.h"
#include "components/source_subsystem/types/ast/node_constant.h"
#include "components/source_subsystem/types/ast/node_if_statement.h"
#include "components/source_subsystem/types/ast/node_not_expression.h"
#include "components/source_subsystem/types/ast/node_print_statement.h"
#include "components/source_subsystem/types/ast/node_procedure.h"
#include "components/source_subsystem/types/ast/node_program.h"
#include "components/source_subsystem/types/ast/node_read_statement.h"
#include "components/source_subsystem/types/ast/node_relational_expression.h"
#include "components/source_subsystem/types/ast/node_statement_list.h"
#include "components/source_subsystem/types/ast/node_variable.h"
#include "components/source_subsystem/types/ast/node_while_statement.h"
#include "components/source_subsystem/types/call_graph/call_graph.h"

namespace source {

DesignExtractor::DesignExtractor(pkb::PkbClientPtr pkb_client) :
    m_pkb_client(std::move(pkb_client)),
    m_call_graph(std::make_shared<CallGraph>()),
    m_visited(StringStream()),
    m_is_uses_stmt(true) {}

void DesignExtractor::IterateAst(ProgramNodePtr &program_node) {
  // iterates AST and populate PKB
  Visit(program_node);
  // populate stuff that can only be done after
  // iterating entire AST
  PostIteratePopulate();
}

void DesignExtractor::PostIteratePopulate() {
  // this method updates Uses and Modifies
  // for Call statements
  StringStream topological_order = m_call_graph->TopoSort();
  if (topological_order.empty()) {
    return;
  }

  unsigned int topological_size = topological_order.size();
  for (unsigned int i = topological_size - 1; i > 0; i--) {
    m_pkb_client->UpdateCallUsesModifies(topological_order.at(i));
  }
}

void DesignExtractor::PopulateFollowsHelper(StatementNodeStream statements, int index) {
  if (index < 1) {
    return;
  }

  String prev_stmt = statements[index - 1]->GetStatementNumber();
  String curr_stmt = statements[index]->GetStatementNumber();

  // Populate Follows
  m_pkb_client->PopulateFollows(prev_stmt, curr_stmt);

  // Populate Follows*
  for (int i = index - 1; i >= 0; i--) {
    prev_stmt = statements[i]->GetStatementNumber();
    m_pkb_client->PopulateFollowsStar(prev_stmt, curr_stmt);
  }
}

void DesignExtractor::PopulateParentHelper(String &stmt_no) {
  if (m_visited.empty()) {
    return;
  }
  m_pkb_client->PopulateParent(m_visited.back(), stmt_no);
}

void DesignExtractor::Visit(ProgramNodePtr &program_node) {
  ProcedureNodeStream procedures = program_node->GetProcedures();
  for (ProcedureNodePtr &procedure : procedures) {
    procedure->Accept(shared_from_this());
  }
}

void DesignExtractor::Visit(ProcedureNodePtr &procedure_node) {
  String proc_name = procedure_node->GetName();
  StatementListNodePtr stmt_list = procedure_node->GetStatementList();

  // update m_curr_proc, needed to be done before Accept()
  // as underlying logic may require the current parsed
  // procedure name
  m_curr_proc = proc_name;

  stmt_list->Accept(shared_from_this());
  m_pkb_client->PopulateProc(proc_name);
}

void DesignExtractor::Visit(StatementListNodePtr &stmt_list_node) {
  StatementNodeStream statements = stmt_list_node->GetStatements();
  for (int i = 0; i < statements.size(); i++) {
    StatementNodePtr statement = statements[i];
    String stmt_no = statement->GetStatementNumber();

    // update m_curr_stmt_no, needed to be done before Accept()
    // as underlying logic may require the current parsed
    // statement number
    m_curr_stmt_no = stmt_no;

    statement->Accept(shared_from_this());
    PopulateFollowsHelper(statements, i);
    PopulateParentHelper(stmt_no);
  }
}

void DesignExtractor::Visit(ReadStatementNodePtr &read_stmt) {
  String stmt_num = read_stmt->GetStatementNumber();
  VariableNodePtr var = read_stmt->GetVariable();
  String var_name = var->GetName();

  m_pkb_client->PopulateTypeOfStmt(stmt_num, READ);
  m_is_uses_stmt = false;
  var->Accept(shared_from_this());
  m_pkb_client->PopulateRead(m_visited, stmt_num, var_name);
}

void DesignExtractor::Visit(PrintStatementNodePtr &print_stmt) {
  String stmt_num = print_stmt->GetStatementNumber();
  VariableNodePtr var = print_stmt->GetVariable();
  String var_name = var->GetName();

  m_pkb_client->PopulateTypeOfStmt(stmt_num, PRINT);
  m_is_uses_stmt = true;
  var->Accept(shared_from_this());
  m_pkb_client->PopulatePrint(m_visited, stmt_num, var_name);
}

void DesignExtractor::Visit(AssignStatementNodePtr &assign_stmt) {
  String stmt_num = assign_stmt->GetStatementNumber();
  VariableNodePtr var = assign_stmt->GetLhs();
  String var_name = var->GetName();
  ExpressionNodePtr expr = assign_stmt->GetRhs();

  m_pkb_client->PopulateTypeOfStmt(stmt_num, ASSIGN);
  m_is_uses_stmt = false;
  var->Accept(shared_from_this());
  m_is_uses_stmt = true;
  expr->Accept(shared_from_this());

  // m_pattern being modified from underlying expr Accept()
  String rhs_expr = m_pattern;
  m_pkb_client->PopulateAssign(m_visited, m_curr_proc, stmt_num, var_name, rhs_expr);
}

void DesignExtractor::Visit(CallStatementNodePtr &call_stmt) {
  String stmt_num = call_stmt->GetStatementNumber();
  String callee_name = call_stmt->GetCalleeName();
  String caller_name = call_stmt->GetCallerName();

  m_pkb_client->PopulateTypeOfStmt(stmt_num, CALL);
  m_pkb_client->PopulateCall(m_visited, stmt_num, caller_name, callee_name);
  m_call_graph->AddEdge(caller_name, callee_name);
}

void DesignExtractor::Visit(WhileStatementNodePtr &while_stmt) {
  String stmt_num = while_stmt->GetStatementNumber();
  ConditionalExpressionNodePtr cond_expr = while_stmt->GetCondition();
  StatementListNodePtr while_stmt_list = while_stmt->GetStatementList();

  m_pkb_client->PopulateTypeOfStmt(stmt_num, WHILE);
  m_visited.push_back(stmt_num);
  m_is_uses_stmt = true;
  cond_expr->Accept(shared_from_this());

  // m_pattern being modified from underlying expr Accept()
  String cond_pattern = m_pattern;

  while_stmt_list->Accept(shared_from_this());
  m_pkb_client->PopulateWhile(m_visited, stmt_num, cond_pattern);
  m_visited.pop_back();
}

void DesignExtractor::Visit(IfStatementNodePtr &if_stmt) {
  String stmt_num = if_stmt->GetStatementNumber();
  ConditionalExpressionNodePtr cond_expr = if_stmt->GetCondition();
  StatementListNodePtr if_stmt_list = if_stmt->GetIfStatementList();
  StatementListNodePtr else_else_list = if_stmt->GetElseStatementList();

  m_pkb_client->PopulateTypeOfStmt(stmt_num, IF);
  m_visited.push_back(stmt_num);
  m_is_uses_stmt = true;
  cond_expr->Accept(shared_from_this());

  // m_pattern being modified from underlying expr Accept()
  String cond_pattern = m_pattern;

  if_stmt_list->Accept(shared_from_this());
  else_else_list->Accept(shared_from_this());
  m_pkb_client->PopulateIf(m_visited, stmt_num, cond_pattern);
  m_visited.pop_back();
}

void DesignExtractor::Visit(BooleanExpressionNodePtr &boolean_expr_node) {
  BooleanOperator boolean_operator = boolean_expr_node->GetOperator();
  ConditionalExpressionNodePtr lhs_cond_expr = boolean_expr_node->GetLhs();
  ConditionalExpressionNodePtr rhs_cond_expr = boolean_expr_node->GetRhs();

  lhs_cond_expr->Accept(shared_from_this());
  // m_pattern being modified from underlying expr Accept()
  String lhs = m_pattern;

  rhs_cond_expr->Accept(shared_from_this());
  // m_pattern being modified from underlying expr Accept()
  String rhs = m_pattern;

  if (boolean_operator == BooleanOperator::AND) {
    m_pattern = lhs + "&&" + rhs;
    return;
  }
  m_pattern = lhs + "||" + rhs;
}

void DesignExtractor::Visit(RelationalExpressionNodePtr &rel_expr_node) {
  RelationOperator relation_operator = rel_expr_node->GetOperator();
  String relation_operator_label = RelationalExpressionNode::GetOperatorLabel(relation_operator);
  ExpressionNodePtr lhs_expr = rel_expr_node->GetLhs();
  ExpressionNodePtr rhs_expr = rel_expr_node->GetRhs();

  lhs_expr->Accept(shared_from_this());
  // m_pattern being modified from underlying expr Accept()
  String lhs = m_pattern;

  rhs_expr->Accept(shared_from_this());
  // m_pattern being modified from underlying expr Accept()
  String rhs = m_pattern;

  m_pattern = "(" + lhs + relation_operator_label + rhs + ")";
}

void DesignExtractor::Visit(NotExpressionNodePtr &not_expr_node) {
  ConditionalExpressionNodePtr cond_expr = not_expr_node->GetExpression();
  cond_expr->Accept(shared_from_this());
}

void DesignExtractor::Visit(CombinationExpressionNodePtr &combination_expr_node) {
  ArithmeticOperator arithmetic_operator = combination_expr_node->GetOperator();
  String arithmetic_operator_label = CombinationExpressionNode::GetOperatorLabel(arithmetic_operator);
  ExpressionNodePtr lhs_expr = combination_expr_node->GetLhs();
  ExpressionNodePtr rhs_expr = combination_expr_node->GetRhs();

  lhs_expr->Accept(shared_from_this());
  // m_pattern being modified from underlying expr Accept()
  String lhs = m_pattern;

  rhs_expr->Accept(shared_from_this());
  // m_pattern being modified from underlying expr Accept()
  String rhs = m_pattern;

  m_pattern = "(" + lhs + arithmetic_operator_label + rhs + ")";
}

void DesignExtractor::Visit(VariableNodePtr &variable_node) {
  String var_name = variable_node->GetName();
  m_pkb_client->PopulateVars(m_visited, m_curr_stmt_no, m_curr_proc, var_name, m_is_uses_stmt);
  m_pattern = variable_node->GetPatternFormat();
}

void DesignExtractor::Visit(ConstantNodePtr &constant_node) {
  String value = constant_node->GetValue();
  m_pkb_client->PopulateConst(value);
  m_pattern = constant_node->GetPatternFormat();
}

}
