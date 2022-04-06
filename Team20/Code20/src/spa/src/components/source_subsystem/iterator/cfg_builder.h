#ifndef SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_ITERATOR_CFG_BUILDER_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_ITERATOR_CFG_BUILDER_H_

#include "components/source_subsystem/source_declarations.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <stack>
#include "components/source_subsystem/types/cfg/cfg.h"
#include "components/source_subsystem/types/cfg/cfg_node.h"

namespace source {

class CfgBuilder : public std::enable_shared_from_this<CfgBuilder> {
 private:
  std::shared_ptr<PkbClient> m_pkb_client;
  StringToCfgNodePtrMap m_cfg_heads_map;

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
  CfgNodePtr Visit(ProgramNodePtr program_node, CfgNodePtr cfg_node);
  CfgNodePtr Visit(ProcedureNodePtr procedure_node, CfgNodePtr cfg_node);
  CfgNodePtr Visit(StatementListNodePtr stmt_list_node, CfgNodePtr cfg_node);
  CfgNodePtr Visit(ReadStatementNodePtr read_stmt, CfgNodePtr cfg_node);
  CfgNodePtr Visit(PrintStatementNodePtr print_stmt, CfgNodePtr cfg_node);
  CfgNodePtr Visit(AssignStatementNodePtr assign_stmt, CfgNodePtr cfg_node);
  CfgNodePtr Visit(CallStatementNodePtr call_stmt, CfgNodePtr cfg_node);
  CfgNodePtr Visit(WhileStatementNodePtr while_stmt, CfgNodePtr cfg_node);
  CfgNodePtr Visit(IfStatementNodePtr if_stmt, CfgNodePtr cfg_node);
};

}

#endif //SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_ITERATOR_CFG_BUILDER_H_
