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

class DesignExtractor {
 private:
  std::shared_ptr<PkbClient> m_pkb_client;
  std::shared_ptr<CallGraph> m_call_graph;
  std::vector<std::string> m_visited;

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
  void Visit(std::shared_ptr<ProgramNode> node);
  void Visit(std::shared_ptr<ProcedureNode> mode);
  void Visit(std::shared_ptr<StatementListNode> node, std::string proc_name);
  void Visit(std::shared_ptr<StatementNode> node, std::string proc_name);
  [[nodiscard]] std::string Visit(std::shared_ptr<ExpressionNode> node, std::string proc_name, bool is_uses);
  [[nodiscard]] std::string Visit(std::shared_ptr<ConditionalExpressionNode> node, std::string proc_name, bool is_uses);
  void Visit(std::shared_ptr<VariableNode> node, std::string proc_name, bool is_uses);
  void Visit(std::shared_ptr<ConstantNode> node, std::string proc_name, bool is_uses);
};

}

#endif //SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_ITERATOR_DESIGN_EXTRACTOR_H_
