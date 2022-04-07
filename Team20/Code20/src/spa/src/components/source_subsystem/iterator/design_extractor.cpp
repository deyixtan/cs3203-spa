#include "design_extractor.h"
#include "components/source_subsystem/pkb_client.h"
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
#include "components/source_subsystem/types/ast/node_statement.h"
#include "components/source_subsystem/types/ast/node_statement_list.h"
#include "components/source_subsystem/types/ast/node_variable.h"
#include "components/source_subsystem/types/ast/node_while_statement.h"
#include "components/source_subsystem/types/call_graph/call_graph.h"

namespace source {

DesignExtractor::DesignExtractor(PkbClientPtr pkb_client) :
    m_pkb_client(std::move(pkb_client)), m_call_graph(CallGraphPtr()), m_visited(StringStream()), m_is_uses(true) {}

void DesignExtractor::IterateAstAndPopulatePkb(ProgramNodePtr &program_node) {
  Visit(program_node);

  StringStream topological_order = m_call_graph->TopoSort();
  for (unsigned int i = topological_order.size() - 1; i >= 0; i--) {
    UpdateCallUsesModifies(topological_order.at(i));
  }
}

CallGraphPtr DesignExtractor::GetCallGraph() {
  return m_call_graph;
}

StringStream DesignExtractor::GetVisited() {
  return m_visited;
}

void DesignExtractor::UpdateCallUsesModifies(String &proc) {
  StringSet uses_vars = m_pkb_client->GetVarUsedByStmt(proc);
  StringSet mod_vars = m_pkb_client->GetVarModByStmt(proc);
  StringSet call_stmts = m_pkb_client->GetCallStmtOf(proc);
  StringSet callers = m_pkb_client->GetCallersOf(proc);

  for (auto call_stmt : call_stmts) {
    StringSet ancestors = m_pkb_client->GetAllAnceOf(call_stmt);
    UpdateCallUses(call_stmt, uses_vars, ancestors, callers);
    UpdateCallModifies(call_stmt, mod_vars, ancestors, callers);
  }
}

void DesignExtractor::UpdateCallUses(String &call_stmt, StringSet &vars, StringSet &ancestors, StringSet &callers) {
  for (auto &var : vars) {
    m_pkb_client->PopulateUses(call_stmt, var);
    for (auto &ance : ancestors) {
      m_pkb_client->PopulateUses(ance, var);
    }
    for (auto &caller : callers) {
      m_pkb_client->PopulateUses(caller, var);
    }
  }
}

void DesignExtractor::UpdateCallModifies(String &call_stmt, StringSet &vars, StringSet &ancestors, StringSet &callers) {
  for (auto &var : vars) {
    m_pkb_client->PopulateModifies(call_stmt, var);
    for (auto &ance : ancestors) {
      m_pkb_client->PopulateModifies(ance, var);
    }
    for (auto &caller : callers) {
      m_pkb_client->PopulateModifies(caller, var);
    }

  }
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

  // populate pkb
  m_curr_proc = proc_name;
  m_pkb_client->PopulateProc(proc_name);

  stmt_list->Accept(shared_from_this());
}

void DesignExtractor::Visit(StatementListNodePtr &stmt_list_node) {
  StatementNodeStream statements = stmt_list_node->GetStatements();
  for (int i = 0; i < statements.size(); ++i) {
    StatementNodePtr statement = statements[i];
    statement->Accept(shared_from_this());

    String stmt_num = statement->GetStatementNumber();
    if (!GetVisited().empty()) {
      m_pkb_client->PopulateParent(GetVisited().back(), stmt_num);
    }

    if (i > 0) {
      String prev_stmt = statements[i - 1]->GetStatementNumber();
      String curr_stmt = statement->GetStatementNumber();
      m_pkb_client->PopulateFollows(prev_stmt, curr_stmt);

      // PopulateFollowsStar
      for (int j = i - 1; j >= 0; --j) {
        prev_stmt = statements[j]->GetStatementNumber();
        m_pkb_client->PopulateFollowsStar(prev_stmt, curr_stmt);
      }
    }
  }
}

void DesignExtractor::Visit(ReadStatementNodePtr &read_stmt) {
  String stmt_num = read_stmt->GetStatementNumber();
  VariableNodePtr var = read_stmt->GetVariable();
  String var_name = var->GetName();

  m_curr_stmt_no = stmt_num;
  m_pkb_client->PopulateTypeOfStmt(stmt_num, READ);
  m_is_uses = false;
  var->Accept(shared_from_this()); // apart from proc_name, true?
  m_pkb_client->PopulateRead(m_visited, stmt_num, var_name);
}

void DesignExtractor::Visit(PrintStatementNodePtr &print_stmt) {
  String stmt_num = print_stmt->GetStatementNumber();
  VariableNodePtr var = print_stmt->GetVariable();
  String var_name = var->GetName();

  m_curr_stmt_no = stmt_num;
  m_pkb_client->PopulateTypeOfStmt(stmt_num, PRINT);
  m_is_uses = true;
  var->Accept(shared_from_this()); // apart from proc_name, true?
  m_pkb_client->PopulatePrint(m_visited, stmt_num, var_name);
}

void DesignExtractor::Visit(AssignStatementNodePtr &assign_stmt) {
  String stmt_num = assign_stmt->GetStatementNumber();
  VariableNodePtr var = assign_stmt->GetLhs();
  String var_name = var->GetName();
  ExpressionNodePtr expr = assign_stmt->GetRhs();

  m_curr_stmt_no = stmt_num;
  m_pkb_client->PopulateTypeOfStmt(stmt_num, ASSIGN);
  m_is_uses = false;
  var->Accept(shared_from_this()); // apart from proc_name, false?
  m_is_uses = true;
  expr->Accept(shared_from_this()); // apart from proc_name, true?
  String rhs_expr = m_pattern;
  m_pkb_client->PopulateAssign(m_visited, m_curr_proc, stmt_num, var_name, rhs_expr);
}

void DesignExtractor::Visit(CallStatementNodePtr &call_stmt) {
  String stmt_num = call_stmt->GetStatementNumber();
  String callee_name = call_stmt->GetCalleeName();
  String caller_name = call_stmt->GetCallerName();

  m_curr_stmt_no = stmt_num;
  m_pkb_client->PopulateTypeOfStmt(stmt_num, CALL);
  m_pkb_client->PopulateCall(m_visited, stmt_num, caller_name, callee_name);
  GetCallGraph()->AddEdge(caller_name, callee_name);
}

void DesignExtractor::Visit(WhileStatementNodePtr &while_stmt) {
  String stmt_num = while_stmt->GetStatementNumber();
  ConditionalExpressionNodePtr cond_expr = while_stmt->GetCondition();
  StatementListNodePtr while_stmt_list = while_stmt->GetStatementList();

  m_curr_stmt_no = stmt_num;
  m_pkb_client->PopulateTypeOfStmt(stmt_num, WHILE);
  GetVisited().push_back(stmt_num);
  // TODO: rename cond_expr2
  m_is_uses = true;
  cond_expr->Accept(shared_from_this()); // apart from proc_name, true?
  String cond_expr2 = m_pattern;
  while_stmt_list->Accept(shared_from_this());
  m_pkb_client->PopulateWhile(m_visited, stmt_num, cond_expr2);
}

void DesignExtractor::Visit(IfStatementNodePtr &if_stmt) {
  String stmt_num = if_stmt->GetStatementNumber();
  ConditionalExpressionNodePtr cond_expr = if_stmt->GetCondition();
  StatementListNodePtr if_stmt_list = if_stmt->GetIfStatementList();
  StatementListNodePtr else_else_list = if_stmt->GetElseStatementList();

  m_curr_stmt_no = stmt_num;
  m_pkb_client->PopulateTypeOfStmt(stmt_num, IF);
  GetVisited().push_back(stmt_num);
  // TODO: rename cond_expr2
  m_is_uses = true;
  cond_expr->Accept(shared_from_this()); // apart from proc_name, true?
  String cond_expr2 = m_pattern;
  if_stmt_list->Accept(shared_from_this());
  else_else_list->Accept(shared_from_this());
  m_pkb_client->PopulateIf(m_visited, stmt_num, cond_expr2);
}

void DesignExtractor::Visit(BooleanExpressionNodePtr &boolean_expr_node) {
  BooleanOperator boolean_operator = boolean_expr_node->GetOperator();
  ConditionalExpressionNodePtr lhs_cond_expr = boolean_expr_node->GetLhs();
  ConditionalExpressionNodePtr rhs_cond_expr = boolean_expr_node->GetRhs();

  lhs_cond_expr->Accept(shared_from_this()); // is_uses?
  String lhs = m_pattern;
  rhs_cond_expr->Accept(shared_from_this()); // is_uses?
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
  String lhs = m_pattern;
  rhs_expr->Accept(shared_from_this());
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
  String lhs = m_pattern;
  rhs_expr->Accept(shared_from_this());
  String rhs = m_pattern;
  m_pattern = "(" + lhs + arithmetic_operator_label + rhs + ")";
}

void DesignExtractor::Visit(VariableNodePtr &variable_node) {
  String var_name = variable_node->GetName();

  m_pkb_client->PopulateVars(m_visited, m_curr_stmt_no, m_curr_proc, var_name, m_is_uses);
  m_pattern = variable_node->GetPatternFormat();
}

void DesignExtractor::Visit(ConstantNodePtr &constant_node) {
  String value = constant_node->GetValue();
  m_pkb_client->PopulateConst(value);
  m_pattern = constant_node->GetPatternFormat();
}

}
