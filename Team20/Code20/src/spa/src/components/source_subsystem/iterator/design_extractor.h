#ifndef SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_ITERATOR_DESIGN_EXTRACTOR_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_ITERATOR_DESIGN_EXTRACTOR_H_

#include "components/source_subsystem/source_declarations.h"
#include <memory>
#include <string>
#include <vector>
#include <stack>
#include "../../pkb/pkb.h"

namespace source {

class PkbClient;
class CallGraph;
class ProgramNode;
class ProcedureNode;
class StatementListNode;
class StatementNode;
class ExpressionNode;
class ConditionalExpressionNode;
class VariableNode;
class ConstantNode;

class DesignExtractor : public std::enable_shared_from_this<DesignExtractor> {
 private:
  std::shared_ptr<PkbClient> m_pkb_client;
  std::shared_ptr<CallGraph> m_call_graph;
  std::vector<std::string> m_visited;
  String m_curr_proc;
  String m_curr_stmt_no;
  bool m_is_uses;
  String m_pattern;

 public:
  explicit DesignExtractor(std::shared_ptr<PkbClient> pkb_client);
  [[nodiscard]] std::shared_ptr<PkbClient> GetPkbClient();
  [[nodiscard]] std::shared_ptr<CallGraph> GetCallGraph();
  [[nodiscard]] std::vector<std::string> &GetVisited();
  void IterateAstAndPopulatePkb(std::shared_ptr<ProgramNode> node);
  void UpdateCallUsesModifies(std::string proc);
  void UpdateCallUses(std::string const &call_stmt,
                      std::unordered_set<std::string> const &vars,
                      std::unordered_set<std::string> const &ancestors,
                      std::unordered_set<std::string> const &callers);
  void UpdateCallModifies(std::string const &call_stmt,
                          std::unordered_set<std::string> const &vars,
                          std::unordered_set<std::string> const &ancestors,
                          std::unordered_set<std::string> const &callers);

  void Visit(ProgramNodePtr program_node);
  void Visit(ProcedureNodePtr procedure_node);
  void Visit(StatementListNodePtr stmt_list_node);
  void Visit(ReadStatementNodePtr read_stmt);
  void Visit(PrintStatementNodePtr print_stmt);
  void Visit(AssignStatementNodePtr assign_stmt);
  void Visit(CallStatementNodePtr call_stmt);
  void Visit(WhileStatementNodePtr while_stmt);
  void Visit(IfStatementNodePtr if_stmt);
  void Visit(BooleanExpressionNodePtr boolean_expr_node);
  void Visit(RelationalExpressionNodePtr rel_expr_node);
  void Visit(NotExpressionNodePtr not_expr_node);
  void Visit(CombinationExpressionNodePtr combination_expr_node);
  void Visit(VariableNodePtr variable_node);
  void Visit(ConstantNodePtr constant_node);
};

}

#endif //SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_ITERATOR_DESIGN_EXTRACTOR_H_
