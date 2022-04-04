#include "cfg_builder.h"
#include "components/source_subsystem/pkb_client.h"
#include "../types/ast/node_program.h"
#include "../types/ast/node_procedure.h"
#include "../types/ast/node_statement_list.h"
#include "../types/ast/node_statement.h"
#include "../types/cfg/cfg.h"
#include "../types/cfg/cfg_node.h"
#include "../types/call_graph/call_graph.h"

namespace source {

CfgBuilder::CfgBuilder(std::shared_ptr<PkbClient> pkb_client) : m_pkb_client(std::move(pkb_client)) {}

std::shared_ptr<PkbClient> CfgBuilder::GetPkbClient() {
  return m_pkb_client;
}

void CfgBuilder::IterateAstAndPopulatePkb(std::shared_ptr<ProgramNode> node) {
  std::unordered_map<std::string, std::shared_ptr<CfgNode>> cfg_map = Visit(std::move(node));
  std::shared_ptr<Cfg> program_cfg = std::make_shared<Cfg>(cfg_map);
  m_pkb_client->PopulateCfg(*program_cfg);
}

void CfgBuilder::IterateCfgAndPopulatePkb(std::shared_ptr<Cfg> root) {
  std::stack<std::shared_ptr<CfgNode>> node_stack;
  std::vector<Statement> prev_stmts;
  std::unordered_set<std::shared_ptr<CfgNode>> visited;
  std::unordered_map<std::string, std::unordered_set<std::string>> next_map;
  std::unordered_map<std::string, std::shared_ptr<CfgNode>> prog = root->GetCfgMap();
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

std::unordered_map<std::string, std::shared_ptr<CfgNode>> CfgBuilder::Visit(std::shared_ptr<ProgramNode> node) {
  return node->Accept(this);
}

std::shared_ptr<CfgNode> CfgBuilder::Visit(std::shared_ptr<ProcedureNode> node, std::shared_ptr<CfgNode> cfg_node) {
  return node->Accept(this, std::move(cfg_node));
}

std::shared_ptr<CfgNode> CfgBuilder::Visit(std::shared_ptr<StatementListNode> node, std::shared_ptr<CfgNode> cfg_node) {
  return node->Accept(this, std::move(cfg_node));
}

std::shared_ptr<CfgNode> CfgBuilder::Visit(std::shared_ptr<StatementNode> node, std::shared_ptr<CfgNode> cfg_node) {
  // runtime polymorphism decides which specific StatementNode's Accept method to invoke
  return node->Accept(this, std::move(cfg_node));
}

}
