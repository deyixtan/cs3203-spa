#include "cfg_builder.h"
#include "components/source_subsystem/pkb_client.h"
#include "components/source_subsystem/types/ast/node_assign_statement.h"
#include "components/source_subsystem/types/ast/node_call_statement.h"
#include "components/source_subsystem/types/ast/node_if_statement.h"
#include "components/source_subsystem/types/ast/node_print_statement.h"
#include "components/source_subsystem/types/ast/node_procedure.h"
#include "components/source_subsystem/types/ast/node_program.h"
#include "components/source_subsystem/types/ast/node_read_statement.h"
#include "components/source_subsystem/types/ast/node_statement_list.h"
#include "components/source_subsystem/types/ast/node_while_statement.h"
#include "components/source_subsystem/types/cfg/cfg_node.h"

namespace source {

CfgBuilder::CfgBuilder(PkbClientPtr pkb_client)
    : m_pkb_client(std::move(pkb_client)), m_cfg_map(StringToCfgNodePtrMap()) {}

void CfgBuilder::IterateAst(ProgramNodePtr &program_node) {
  // iterates AST and populate PKB
  Visit(program_node);
}

void CfgBuilder::IterateCfg() {
  CfgNodeStack node_stack;
  CfgNodeStatementStream prev_stmts;
  CfgNodeSet visited;
  StringToStringSetMap next_map;

  StringToCfgNodePtrMap cfg_map = m_pkb_client->GetProgramCfg()->GetCfgMap();
  for (auto &cfg : cfg_map) {
    CfgNodePtr cfg_head = cfg.second; // root node of cfg
    CfgProcessHandler(cfg_head, node_stack, prev_stmts, visited, next_map);
  }
}

void CfgBuilder::Visit(ProgramNodePtr &program_node) {
  ProcedureNodeStream procedures = program_node->GetProcedures();
  for (ProcedureNodePtr &procedure : procedures) {
    m_curr_cfg_node = std::make_shared<CfgNode>();
    CfgNodePtr head = m_curr_cfg_node;
    procedure->Accept(shared_from_this());

    // create empty tail node and link to end of current Cfg
    if (!m_curr_cfg_node->GetStatementList().empty()) {
      CfgNodePtr dummy = std::make_shared<CfgNode>();
      m_curr_cfg_node->AddNext(dummy);
    }
    m_cfg_map.insert({procedure->GetName(), head});
  }

  CfgPtr program_cfg = std::make_shared<Cfg>(m_cfg_map);
  m_pkb_client->PopulateCfg(program_cfg);
}

void CfgBuilder::Visit(ProcedureNodePtr &procedure_node) {
  StatementListNodePtr stmt_list = procedure_node->GetStatementList();
  stmt_list->Accept(shared_from_this());
}

void CfgBuilder::Visit(StatementListNodePtr &stmt_list_node) {
  StatementNodeStream statements = stmt_list_node->GetStatements();
  for (StatementNodePtr &statement : statements) {
    statement->Accept(shared_from_this());
  }
}

void CfgBuilder::Visit(ReadStatementNodePtr &read_stmt) {
  String stmt_num = read_stmt->GetStatementNumber();
  m_curr_cfg_node->AddStatement(StmtType::READ, stmt_num);
}

void CfgBuilder::Visit(PrintStatementNodePtr &print_stmt) {
  String stmt_num = print_stmt->GetStatementNumber();
  m_curr_cfg_node->AddStatement(StmtType::PRINT, stmt_num);
}

void CfgBuilder::Visit(AssignStatementNodePtr &assign_stmt) {
  String stmt_num = assign_stmt->GetStatementNumber();
  m_curr_cfg_node->AddStatement(StmtType::ASSIGN, stmt_num);
}

void CfgBuilder::Visit(CallStatementNodePtr &call_stmt) {
  String stmt_num = call_stmt->GetStatementNumber();
  m_curr_cfg_node->AddStatement(StmtType::CALL, stmt_num);
}

void CfgBuilder::Visit(WhileStatementNodePtr &while_stmt) {
  String stmt_num = while_stmt->GetStatementNumber();
  StatementListNodePtr while_stmt_list = while_stmt->GetStatementList();

  // add dummy node for this Visit() to work on
  bool is_empty_cfg_node = m_curr_cfg_node->GetStatementList().empty();
  if (!is_empty_cfg_node) {
    CfgNodePtr condition_node = std::make_shared<CfgNode>();
    m_curr_cfg_node->AddNext(condition_node);
    m_curr_cfg_node = condition_node;
  }

  CfgNodePtr cfg_node = m_curr_cfg_node; // ref to condition node
  CfgNodePtr body_node = std::make_shared<CfgNode>();
  CfgNodePtr next_node = std::make_shared<CfgNode>();

  m_curr_cfg_node->AddStatement(StmtType::WHILE, stmt_num);
  m_curr_cfg_node->AddNext(body_node);
  m_curr_cfg_node->AddNext(next_node);

  // Populate body_node
  m_curr_cfg_node = body_node;
  while_stmt_list->Accept(shared_from_this());

  // Points tail of body node back to next_node (condition) to create a loop
  m_curr_cfg_node->AddNext(cfg_node);
  m_curr_cfg_node = next_node;
}

void CfgBuilder::Visit(IfStatementNodePtr &if_stmt) {
  String stmt_num = if_stmt->GetStatementNumber();
  StatementListNodePtr if_stmt_list = if_stmt->GetIfStatementList();
  StatementListNodePtr else_stmt_list = if_stmt->GetElseStatementList();

  // add dummy node for this Visit() to work on
  bool is_empty_cfg_node = m_curr_cfg_node->GetStatementList().empty();
  if (!is_empty_cfg_node) {
    CfgNodePtr condition_node = std::make_shared<CfgNode>();
    m_curr_cfg_node->AddNext(condition_node);
    m_curr_cfg_node = condition_node;
  }

  CfgNodePtr if_node = std::make_shared<CfgNode>();
  CfgNodePtr else_node = std::make_shared<CfgNode>();
  CfgNodePtr next_node = std::make_shared<CfgNode>();

  m_curr_cfg_node->AddStatement(StmtType::IF, stmt_num);
  m_curr_cfg_node->AddNext(if_node);
  m_curr_cfg_node->AddNext(else_node);

  // Populate if_node
  m_curr_cfg_node = if_node;
  if_stmt_list->Accept(shared_from_this());
  m_curr_cfg_node->AddNext(next_node);

  // Populate else_node
  m_curr_cfg_node = else_node;
  else_stmt_list->Accept(shared_from_this());
  m_curr_cfg_node->AddNext(next_node);

  // Points tail of if-block and else-block to next_node
  m_curr_cfg_node = next_node;
}

void CfgBuilder::CfgProcessHandler(CfgNodePtr &curr_proc,
                                   CfgNodeStack &node_stack,
                                   CfgNodeStatementStream &prev_stmts,
                                   CfgNodeSet &visited,
                                   StringToStringSetMap &next_map) {
  node_stack.push(curr_proc);

  // per cfg logic
  while (!node_stack.empty()) {
    CfgNodePtr curr = node_stack.top();
    node_stack.pop();
    visited.insert(curr);

    CfgNodeStatementStream curr_stmts = curr->GetStatementList(); // get all stmt in node
    CfgNodeStream next_nodes = curr->GetNextList(); // get all possible next nodes

    // check if actual dummy node
    if (curr_stmts.empty() && next_nodes.empty()) {
      if (node_stack.empty()) {
        break;
      }
      continue;
    }

    // node with more than one statement
    MultipleStmtsNodeHandler(curr_stmts, next_map);

    // recurse until next_node.front() != dummy node
    while (!next_nodes.empty() && next_nodes.front()->GetStatementList().empty()) {
      next_nodes = next_nodes.front()->GetNextList(); // becomes next_nodes = 11
    }

    for (auto &desc : next_nodes) {
      NextNodeHandler(desc, node_stack, curr_stmts, visited, next_map);
    }
  }
}

void CfgBuilder::MultipleStmtsNodeHandler(CfgNodeStatementStream &curr_stmts, StringToStringSetMap &next_map) {
  int start = 0;
  int next = 1;
  while (curr_stmts.size() > next) {
    m_pkb_client->PopulateNext(curr_stmts[start].stmt_no, curr_stmts[next].stmt_no);
    start++;
    next++;
  }
}

void CfgBuilder::NextNodeHandler(CfgNodePtr &desc,
                                 CfgNodeStack &node_stack,
                                 CfgNodeStatementStream &curr_stmts,
                                 CfgNodeSet &visited,
                                 StringToStringSetMap &next_map) {
  if (!curr_stmts.empty()) {
    // force desc to legit node
    while (desc->GetStatementList().empty() && !desc->GetNextList().empty()) {
      desc = desc->GetNextList().front();
    }

    CfgNodeStatementStream next_stmts = desc->GetStatementList();
    if (!next_stmts.empty()) {
      m_pkb_client->PopulateNext(curr_stmts[curr_stmts.size() - 1].stmt_no, next_stmts.front().stmt_no);
    }
  }

  if (visited.find(desc) == visited.end()) {
    node_stack.push(desc);
  }
}

}
