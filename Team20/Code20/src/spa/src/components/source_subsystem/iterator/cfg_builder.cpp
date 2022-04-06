#include "cfg_builder.h"
#include "components/pkb/pkb.h"
#include "components/source_subsystem/pkb_client.h"
#include "../types/ast/node_program.h"
#include "../types/ast/node_procedure.h"
#include "../types/ast/node_statement_list.h"
#include "../types/ast/node_statement.h"
#include "../types/cfg/cfg.h"
#include "../types/cfg/cfg_node.h"
#include "../types/call_graph/call_graph.h"

namespace source {

CfgBuilder::CfgBuilder(PkbClientPtr pkb_client)
    : m_pkb_client(std::move(pkb_client)), m_cfg_heads_map(StringToCfgNodePtrMap()) {}

std::shared_ptr<PkbClient> CfgBuilder::GetPkbClient() {
  return m_pkb_client;
}

void CfgBuilder::IterateAstAndPopulatePkb(std::shared_ptr<ProgramNode> node) {
  std::unordered_map<std::string, std::shared_ptr<CfgNode>> cfg_map = Visit(std::move(node));
  std::shared_ptr<Cfg> program_cfg = std::make_shared<Cfg>(cfg_map);
  m_pkb_client->PopulateCfg(*program_cfg);
}

void CfgBuilder::IterateCfgAndPopulatePkb() {
  std::stack<std::shared_ptr<CfgNode>> node_stack;
  std::vector<Statement> prev_stmts;
  std::unordered_set<std::shared_ptr<CfgNode>> visited;
  std::unordered_map<std::string, std::unordered_set<std::string>> next_map;
  std::unordered_map<std::string, std::shared_ptr<CfgNode>> prog = m_pkb_client->GetPKB()->GetProgCfg()->GetCfgMap();
  for (auto proc : prog) {
    std::shared_ptr<CfgNode> curr_proc = proc.second; // root node of cfg
    CfgProcessHandler(curr_proc, node_stack, prev_stmts, visited, next_map);
  }
}

void CfgBuilder::CfgProcessHandler(std::shared_ptr<CfgNode> &curr_proc,
                                   std::stack<std::shared_ptr<CfgNode>> &node_stack,
                                   std::vector<Statement> &prev_stmts,
                                   std::unordered_set<std::shared_ptr<CfgNode>> &visited,
                                   std::unordered_map<std::string, std::unordered_set<std::string>> &next_map) {
  node_stack.push(curr_proc);

  // per cfg logic
  while (!node_stack.empty()) {
    std::shared_ptr<CfgNode> curr = node_stack.top();
    node_stack.pop();
    visited.insert(curr);

    std::vector<Statement> curr_stmts = curr->GetStatementList(); // get all stmt in node
    std::vector<std::shared_ptr<CfgNode>> next_nodes = curr->GetDescendants(); // get all possible next nodes

    // check if actual dummy node
    if (curr_stmts.empty() && next_nodes.empty()) {
      if (node_stack.empty()) {
        break;
      } else {
        continue;
      }
    }

    // node with more than one statement
    MultipleStmtsNodeHandler(curr_stmts, next_map);

    // recurse until next_node.front() != dummy node
    while (!next_nodes.empty() && next_nodes.front()->GetStatementList().empty()) {
      next_nodes = next_nodes.front()->GetDescendants(); // becomes next_nodes = 11
    }

    for (auto &desc : next_nodes) {
      NextNodeHandler(desc, node_stack, curr_stmts, visited, next_map);
    }
  }
}

void CfgBuilder::MultipleStmtsNodeHandler(std::vector<Statement> &curr_stmts,
                                          std::unordered_map<std::string,
                                                             std::unordered_set<std::string>> &next_map) {
  int start = 0;
  int next = 1;
  while (curr_stmts.size() > next) {
    m_pkb_client->PopulateNext(curr_stmts[start].stmt_no, curr_stmts[next].stmt_no);
    start++;
    next++;
  }
}

void CfgBuilder::NextNodeHandler(std::shared_ptr<CfgNode> &desc,
                                 std::stack<std::shared_ptr<CfgNode>> &node_stack,
                                 std::vector<Statement> &curr_stmts,
                                 std::unordered_set<std::shared_ptr<CfgNode>> &visited,
                                 std::unordered_map<std::string,
                                                    std::unordered_set<std::string>> &next_map) {
  if (!curr_stmts.empty()) {
    // force desc to legit node
    while (desc->GetStatementList().empty() && !desc->GetDescendants().empty()) {
      desc = desc->GetDescendants().front();
    }

    std::vector<Statement> next_stmts = desc->GetStatementList();
    if (!next_stmts.empty()) {
      m_pkb_client->PopulateNext(curr_stmts[curr_stmts.size() - 1].stmt_no, next_stmts.front().stmt_no);
    }
  }
  if (visited.find(desc) == visited.end()) {
    node_stack.push(desc);
  }
}

void CfgBuilder::Visit(ProgramNodePtr program_node) {
  ProcedureNodeStream procedures = program_node->GetProcedures();
  for (ProcedureNodePtr &procedure : procedures) {
    CfgNodePtr head = std::make_shared<CfgNode>();
    CfgNodePtr tail = procedure->Accept(procedure, head);
    if (!tail->GetStatementList().empty()) {
      CfgNodePtr dummy = std::make_shared<CfgNode>();
      tail->AddNext(dummy);
    }
    m_cfg_heads_map.insert({procedure->GetName(), head});
  }
  // return heads;
}


void CfgBuilder::Visit(ProcedureNodePtr procedure_node) {
  StatementListNodePtr stmt_list = procedure_node->GetStatementList();
  stmt_list->Accept(shared_from_this());
}

void CfgBuilder::Visit(StatementListNodePtr stmt_list_node) {
  StatementNodeStream statements = stmt_list_node->GetStatements();
  for (StatementNodePtr &statement : statements) {
    bool is_empty_cfg_node = cfg_node->GetStatementList().empty();
    if ((statement->GetStatementType() == IF || statement->GetStatementType() == WHILE) && !is_empty_cfg_node) {
      CfgNodePtr condition_node = std::make_shared<CfgNode>();
      cfg_node->AddNext(condition_node);
      cfg_node = condition_node;
    }
    cfg_node = statement->Accept(shared_from_this());
  }
  // return cfg_node
}

void CfgBuilder::Visit(ReadStatementNodePtr read_stmt) {
  String stmt_num = read_stmt->GetStatementNumber();
  cfg_node->AddStatement(StmtType::READ, stmt_num);
  //return cfg_node;
}

void CfgBuilder::Visit(PrintStatementNodePtr print_stmt) {
  String stmt_num = print_stmt->GetStatementNumber();
  cfg_node->AddStatement(StmtType::PRINT, stmt_num);
  //return cfg_node;
}

void CfgBuilder::Visit(AssignStatementNodePtr assign_stmt) {
  String stmt_num = assign_stmt->GetStatementNumber();
  cfg_node->AddStatement(StmtType::ASSIGN, stmt_num);
  //return cfg_node;
}

void CfgBuilder::Visit(CallStatementNodePtr call_stmt) {
  String stmt_num = call_stmt->GetStatementNumber();
  cfg_node->AddStatement(StmtType::CALL, stmt_num);
  //return cfg_node;
}

void CfgBuilder::Visit(WhileStatementNodePtr while_stmt) {
  String stmt_num = while_stmt->GetStatementNumber();
  StatementListNodePtr while_stmt_list = while_stmt->GetStatementList();

  //  CfgNodePtr condition_node = std::make_shared<CfgNode>();
  //  cfg_node->AddNext(condition_node);
  //  cfg_node = condition_node;
  CfgNodePtr body_node = std::make_shared<CfgNode>();
  CfgNodePtr next_node = std::make_shared<CfgNode>();
  cfg_node->AddStatement(StmtType::WHILE, stmt_num);
  cfg_node->AddNext(body_node);
  cfg_node->AddNext(next_node);
  body_node = while_stmt_list->Accept(shared_from_this());
  body_node->AddNext(cfg_node);
  //return next_node;
}

void CfgBuilder::Visit(IfStatementNodePtr if_stmt) {
  String stmt_num = if_stmt->GetStatementNumber();
  StatementListNodePtr if_stmt_list = if_stmt->GetIfStatementList();
  StatementListNodePtr else_else_list = if_stmt->GetElseStatementList();

  //  CfgNodePtr condition_node = std::make_shared<CfgNode>();
  //  cfg_node->AddNext(condition_node);
  //  cfg_node = condition_node;
  CfgNodePtr if_node = std::make_shared<CfgNode>();
  CfgNodePtr else_node = std::make_shared<CfgNode>();
  CfgNodePtr next_node = std::make_shared<CfgNode>();
  cfg_node->AddStatement(StmtType::IF, stmt_num);
  cfg_node->AddNext(if_node);
  cfg_node->AddNext(else_node);
  if_node = if_stmt_list->Accept(shared_from_this());
  if_node->AddNext(next_node);
  else_node = else_else_list->Accept(shared_from_this());
  else_node->AddNext(next_node);
  //return next_node;
}

}
