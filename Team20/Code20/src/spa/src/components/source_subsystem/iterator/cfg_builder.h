#ifndef CFG_BUILDER_H
#define CFG_BUILDER_H

#include "components/source_subsystem/source_declarations.h"
#include <stack>

namespace source {

class CfgBuilder : public std::enable_shared_from_this<CfgBuilder> {
 private:
  PkbClientPtr m_pkb_client;
  StringToCfgNodePtrMap m_cfg_map;

 public:
  explicit CfgBuilder(PkbClientPtr m_pkb_client);
  void IterateAst(ProgramNodePtr node);
  void IterateCfg();
  void CfgProcessHandler(CfgNodePtr &curr_proc,
                         std::stack<CfgNodePtr> &node_stack,
                         CfgNodeStatementStream &prev_stmts,
                         CfgNodeSet &visited,
                         StringToStringSetMap &next_map);
  void MultipleStmtsNodeHandler(CfgNodeStatementStream &curr_stmts, StringToStringSetMap &next_map);
  void NextNodeHandler(CfgNodePtr &desc,
                       std::stack<CfgNodePtr> &node_stack,
                       CfgNodeStatementStream &curr_stmts,
                       CfgNodeSet &visited,
                       StringToStringSetMap &next_map);
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

#endif //CFG_BUILDER_H
