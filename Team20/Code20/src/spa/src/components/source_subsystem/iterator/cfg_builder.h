#ifndef CFG_BUILDER_H
#define CFG_BUILDER_H

#include "components/source_subsystem/source_declarations.h"

namespace source {

class CfgBuilder : public std::enable_shared_from_this<CfgBuilder> {
 private:
  PkbClientPtr m_pkb_client;
  StringToCfgNodePtrMap m_cfg_map;
  CfgNodePtr m_curr_cfg_node;

 private:
  void CfgProcessHandler(CfgNodePtr &curr_proc,
                         CfgNodeStack &node_stack,
                         CfgNodeStatementStream &prev_stmts,
                         CfgNodeSet &visited,
                         StringToStringSetMap &next_map,
                         String proc_name);
  void MultipleStmtsNodeHandler(CfgNodeStatementStream &curr_stmts, StringToStringSetMap &next_map, String proc_name);
  void NextNodeHandler(CfgNodePtr &desc,
                       CfgNodeStack &node_stack,
                       CfgNodeStatementStream &curr_stmts,
                       CfgNodeSet &visited,
                       StringToStringSetMap &next_map,
                       String proc_name);

 public:
  explicit CfgBuilder(PkbClientPtr m_pkb_client);
  void IterateAst(ProgramNodePtr &program_node);
  void IterateCfg();
  void Visit(ProgramNodePtr &program_node);
  void Visit(ProcedureNodePtr &procedure_node);
  void Visit(StatementListNodePtr &stmt_list_node);
  void Visit(ReadStatementNodePtr &read_stmt);
  void Visit(PrintStatementNodePtr &print_stmt);
  void Visit(AssignStatementNodePtr &assign_stmt);
  void Visit(CallStatementNodePtr &call_stmt);
  void Visit(WhileStatementNodePtr &while_stmt);
  void Visit(IfStatementNodePtr &if_stmt);
};

}

#endif //CFG_BUILDER_H