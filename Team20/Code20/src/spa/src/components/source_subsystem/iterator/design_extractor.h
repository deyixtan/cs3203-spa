#ifndef SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_ITERATOR_DESIGN_EXTRACTOR_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_ITERATOR_DESIGN_EXTRACTOR_H_

#include <memory>
#include <string>
#include <vector>
#include <stack>
#include "components/source_subsystem/types/cfg/cfg.h"
#include "../../pkb/pkb.h"

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
  void IterateCfgAndPopulatePkb(std::shared_ptr<Cfg> root);
  void CfgProcessHandler(std::shared_ptr<CfgNode> &curr_proc,
                         std::stack<std::shared_ptr<CfgNode>> &node_stack,
                         std::vector<std::string> &prev_stmts,
                         std::unordered_set<std::shared_ptr<CfgNode>> &visited,
                         std::unordered_map<std::string, std::unordered_set<std::string>> &next_map);
  void MultipleStmtsNodeHandler(std::vector<std::string> &curr_stmts,
                                std::unordered_map<std::string,
                                                   std::unordered_set<std::string>> &next_map);
  void NextNodeHandler(std::shared_ptr<CfgNode> &desc,
                       std::stack<std::shared_ptr<CfgNode>> &node_stack,
                       std::vector<std::string> &curr_stmts,
                       std::unordered_set<std::shared_ptr<CfgNode>> &visited,
                       std::unordered_map<std::string,
                                          std::unordered_set<std::string>> &next_map);
  void UpdateCallUsesMod(std::string proc);
  void Visit(std::shared_ptr<ProgramNode> node);
  void Visit(std::shared_ptr<ProcedureNode> nod);
  void Visit(std::shared_ptr<StatementListNode> node, std::string proc_name);
  void Visit(std::shared_ptr<StatementNode> node, std::string proc_name);
  [[nodiscard]] std::string Visit(std::shared_ptr<ExpressionNode> node, std::string proc_name, bool is_uses);
  [[nodiscard]] std::string Visit(std::shared_ptr<ConditionalExpressionNode> node, std::string proc_name, bool is_uses);
  void Visit(std::shared_ptr<VariableNode> node, std::string proc_name, bool is_uses);
  void Visit(std::shared_ptr<ConstantNode> node, std::string proc_name, bool is_uses);
};

#endif //SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_ITERATOR_DESIGN_EXTRACTOR_H_
