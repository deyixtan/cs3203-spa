#ifndef DESIGN_EXTRACTOR_H
#define DESIGN_EXTRACTOR_H

#include "components/source_subsystem/source_declarations.h"

namespace source {

class DesignExtractor : public std::enable_shared_from_this<DesignExtractor> {
 private:
  PkbClientPtr m_pkb_client;
  CallGraphPtr m_call_graph;
  // used to maintain state
  // while visiting AST nodes
  StringStream m_visited;
  String m_curr_proc;
  String m_curr_stmt_no;
  String m_pattern;
  bool m_is_uses_stmt; // true -> uses, false -> modifies/anything else

 private:
  void PostIteratePopulate();
  void PopulateFollowsHelper(StatementNodeStream statements, int index);
  void PopulateParentHelper(String stmt_no);

 public:
  explicit DesignExtractor(PkbClientPtr pkb_client);
  void IterateAst(ProgramNodePtr &program_node);
  void Visit(ProgramNodePtr &program_node);
  void Visit(ProcedureNodePtr &procedure_node);
  void Visit(StatementListNodePtr &stmt_list_node);
  void Visit(ReadStatementNodePtr &read_stmt);
  void Visit(PrintStatementNodePtr &print_stmt);
  void Visit(AssignStatementNodePtr &assign_stmt);
  void Visit(CallStatementNodePtr &call_stmt);
  void Visit(WhileStatementNodePtr &while_stmt);
  void Visit(IfStatementNodePtr &if_stmt);
  void Visit(BooleanExpressionNodePtr &boolean_expr_node);
  void Visit(RelationalExpressionNodePtr &rel_expr_node);
  void Visit(NotExpressionNodePtr &not_expr_node);
  void Visit(CombinationExpressionNodePtr &combination_expr_node);
  void Visit(VariableNodePtr &variable_node);
  void Visit(ConstantNodePtr &constant_node);
};

}

#endif //DESIGN_EXTRACTOR_H
