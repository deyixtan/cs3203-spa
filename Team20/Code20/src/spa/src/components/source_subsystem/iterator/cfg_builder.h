#ifndef SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_ITERATOR_CFG_BUILDER_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_ITERATOR_CFG_BUILDER_H_

#include <memory>
#include <string>
#include <unordered_map>
#include <stack>
#include "components/source_subsystem/types/cfg/cfg.h"
#include "components/source_subsystem/types/cfg/cfg_node.h"
#include "components/source_subsystem/source_declarations.h"

namespace source {

class CfgBuilder {
 private:
  std::shared_ptr<PkbClient> m_pkb_client;

 public:
  explicit CfgBuilder(std::shared_ptr<PkbClient> m_pkb_client);
  [[nodiscard]] std::shared_ptr<PkbClient> GetPkbClient();
  void IterateAstAndPopulatePkb(std::shared_ptr<ProgramNode> node);
  void IterateCfgAndPopulatePkb();
  void CfgProcessHandler(std::shared_ptr<CfgNode> &curr_proc,
                         std::stack<std::shared_ptr<CfgNode>> &node_stack,
                         std::vector<Statement> &prev_stmts,
                         std::unordered_set<std::shared_ptr<CfgNode>> &visited,
                         std::unordered_map<std::string, std::unordered_set<std::string>> &next_map);
  void MultipleStmtsNodeHandler(std::vector<Statement> &curr_stmts,
                                std::unordered_map<std::string,
                                std::unordered_set<std::string>> &next_map);
  void NextNodeHandler(std::shared_ptr<CfgNode> &desc,
                       std::stack<std::shared_ptr<CfgNode>> &node_stack,
                       std::vector<Statement> &curr_stmts,
                       std::unordered_set<std::shared_ptr<CfgNode>> &visited,
                       std::unordered_map<std::string,
                       std::unordered_set<std::string>> &next_map);
  [[nodiscard]] std::unordered_map<std::string, std::shared_ptr<CfgNode>> Visit(std::shared_ptr<ProgramNode> node);
  [[nodiscard]] std::shared_ptr<CfgNode> Visit(std::shared_ptr<ProcedureNode> node, std::shared_ptr<CfgNode> cfg_node);
  [[nodiscard]] std::shared_ptr<CfgNode> Visit(std::shared_ptr<StatementListNode> node,
                                               std::shared_ptr<CfgNode> cfg_node);
  [[nodiscard]] std::shared_ptr<CfgNode> Visit(std::shared_ptr<StatementNode> node, std::shared_ptr<CfgNode> cfg_node);
};

}

#endif //SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_ITERATOR_CFG_BUILDER_H_
