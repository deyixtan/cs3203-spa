#include "design_extractor.h"
#include "../call_graph/call_graph.h"
#include "../types/ast/node_program.h"
#include "../types/ast/node_procedure.h"
#include "../types/ast/node_statement_list.h"
#include "../types/ast/node_statement.h"
#include "../types/ast/node_conditional_expression.h"
#include "../types/ast/node_variable.h"

#include "../types/cfg/cfg_node.h"
#include "../types/ast/node_constant.h"

DesignExtractor::DesignExtractor(std::shared_ptr<PkbClient> pkb_client)
    : m_pkb_client(std::move(pkb_client)),
      m_call_graph(std::make_shared<CallGraph>()),
      m_visited(std::vector<std::string>()) {}

std::shared_ptr<PkbClient> DesignExtractor::GetPkbClient() {
  return m_pkb_client;
}

std::shared_ptr<CallGraph> DesignExtractor::GetCallGraph() {
  return m_call_graph;
}

std::vector<std::string> &DesignExtractor::GetVisited() {
  return m_visited;
}

void DesignExtractor::IterateAstAndPopulatePkb(std::shared_ptr<ProgramNode> node) {
  Visit(std::move(node));
  std::vector<std::string> topo_order = m_call_graph->TopoSort();
  for (int i = topo_order.size() - 1; i >= 0; i--) {
    UpdateCallUsesModifies(topo_order.at(i));
  }
}

void DesignExtractor::UpdateCallUsesModifies(std::string proc) {
  std::unordered_set<std::string> uses_vars = m_pkb_client->GetPKB()->GetUsageStore()->GetVarUsedByProc(proc);
  std::unordered_set<std::string> mod_vars = m_pkb_client->GetPKB()->GetModifyStore()->GetVarModByProc(proc);
  std::unordered_set<std::string> call_stmts = m_pkb_client->GetPKB()->GetCallStore()->GetCallStmtOf(proc);
  std::unordered_set<std::string> callers = m_pkb_client->GetPKB()->GetCallStore()->GetCallersOf(proc);

  for (auto &call_stmt : call_stmts) {
    std::unordered_set<std::string> ancestors = m_pkb_client->GetPKB()->GetParentStore()->GetAllAnceOf(call_stmt);

    UpdateCallUses(call_stmt, uses_vars, ancestors, callers);
    UpdateCallModifies(call_stmt, mod_vars, ancestors, callers);
  }
}

void DesignExtractor::UpdateCallUses(std::string const &call_stmt,
                                     std::unordered_set<std::string> const &vars,
                                     std::unordered_set<std::string> const &ancestors,
                                     std::unordered_set<std::string> const &callers) {
  for (auto &var : vars) {
    m_pkb_client->GetPKB()->GetUsageStore()->AddStmtVar(call_stmt, var);
    for (auto &ance : ancestors) {
      m_pkb_client->GetPKB()->GetUsageStore()->AddStmtVar(ance, var);
    }
    for (auto &caller : callers) {
      m_pkb_client->GetPKB()->GetUsageStore()->AddProcVar(caller, var);
    }
  }
}

void DesignExtractor::UpdateCallModifies(std::string const &call_stmt,
                                         std::unordered_set<std::string> const &vars,
                                         std::unordered_set<std::string> const &ancestors,
                                         std::unordered_set<std::string> const &callers) {
  for (auto &var : vars) {
    m_pkb_client->GetPKB()->GetModifyStore()->AddStmtVar(call_stmt, var);
    for (auto &ance : ancestors) {
      m_pkb_client->GetPKB()->GetModifyStore()->AddStmtVar(ance, var);
    }
    for (auto &caller : callers) {
      m_pkb_client->GetPKB()->GetModifyStore()->AddProcVar(caller, var);
    }
  }
}

void DesignExtractor::IterateCfgAndPopulatePkb(std::shared_ptr<Cfg> root) {
  std::stack<std::shared_ptr<CfgNode>> node_stack;
  std::vector<std::string> prev_stmts;
  std::unordered_set<std::shared_ptr<CfgNode>> visited;
  std::unordered_map<std::string, std::unordered_set<std::string>> next_map;
  std::unordered_map<std::string, std::shared_ptr<CfgNode>> prog = root->GetCfgMap();
  for (auto proc : prog) {
    std::shared_ptr<CfgNode> curr_proc = proc.second; // root node of cfg
    CfgProcessHandler(curr_proc, node_stack, prev_stmts, visited, next_map);
  }
  m_pkb_client->PopulateNext(next_map);
}

void DesignExtractor::CfgProcessHandler(std::shared_ptr<CfgNode> &curr_proc,
                                        std::stack<std::shared_ptr<CfgNode>> &node_stack,
                                        std::vector<std::string> &prev_stmts,
                                        std::unordered_set<std::shared_ptr<CfgNode>> &visited,
                                        std::unordered_map<std::string, std::unordered_set<std::string>> &next_map) {
  node_stack.push(curr_proc);

  // per cfg logic
  while (node_stack.size() > 0) {
    std::shared_ptr<CfgNode> curr = node_stack.top();
    node_stack.pop();
    visited.insert(curr);

    std::vector<std::string> curr_stmts = curr->GetStatementList(); // get all stmt in node
    std::vector<std::shared_ptr<CfgNode>> next_nodes = curr->GetDescendants(); // get all possible next nodes

    // check if actual dummy node
    if (curr_stmts.size() == 0 && next_nodes.size() == 0) {
      if (node_stack.size() == 0) {
        break;
      } else {
        continue;
      }
    }

    // node with more than one statement
    MultipleStmtsNodeHandler(curr_stmts, next_map);

    // recurse until next_node.front() != dummy node
    while (next_nodes.size() > 0 && next_nodes.front()->GetStatementList().size() == 0) {
      next_nodes = next_nodes.front()->GetDescendants(); // becomes next_nodes = 11
    }

    for (auto &desc : next_nodes) {
      NextNodeHandler(desc, node_stack, curr_stmts, visited, next_map);
    }
  }
}

void DesignExtractor::MultipleStmtsNodeHandler(std::vector<std::string> &curr_stmts,
                                               std::unordered_map<std::string,
                                                                  std::unordered_set<std::string>> &next_map) {
  int start = 0;
  int next = 1;
  while (curr_stmts.size() > next) {
    // check if first statement is inside next_map
    if (next_map.find(curr_stmts[start]) == next_map.end()) {
      std::unordered_set<std::string> nextSet = std::unordered_set<std::string>();
      nextSet.insert(curr_stmts[next]);
      next_map.insert({curr_stmts[start], nextSet});
    } else {
      std::unordered_set<std::string> vals = next_map[curr_stmts[start]];
      vals.insert(curr_stmts[next]);
    }
    start++;
    next++;
  }
}

void DesignExtractor::NextNodeHandler(std::shared_ptr<CfgNode> &desc,
                                      std::stack<std::shared_ptr<CfgNode>> &node_stack,
                                      std::vector<std::string> &curr_stmts,
                                      std::unordered_set<std::shared_ptr<CfgNode>> &visited,
                                      std::unordered_map<std::string,
                                                         std::unordered_set<std::string>> &next_map) {
  if (curr_stmts.size() > 0) {
    if (next_map.find(curr_stmts[curr_stmts.size() - 1]) == next_map.end()) {
      next_map.insert({curr_stmts[curr_stmts.size() - 1], std::unordered_set<std::string>()});
    }

    // force desc to legit node
    while (desc->GetStatementList().size() == 0 && desc->GetDescendants().size() > 0) {
      desc = desc->GetDescendants().front();
    }

    std::vector<std::string> next_stmts = desc->GetStatementList();
    std::unordered_set<std::string> vals = next_map[curr_stmts[curr_stmts.size() - 1]];
    if (next_stmts.size() > 0) {
      vals.insert(next_stmts.front());
      next_map[curr_stmts[curr_stmts.size() - 1]] = vals;
    }
  }
  if (visited.find(desc) == visited.end()) {
    node_stack.push(desc);
  }
}

void DesignExtractor::Visit(std::shared_ptr<ProgramNode> node) {
  node->Accept(this);
}

void DesignExtractor::Visit(std::shared_ptr<ProcedureNode> node) {
  node->Accept(this);
}

void DesignExtractor::Visit(std::shared_ptr<StatementListNode> node, std::string proc_name) {
  node->Accept(this, proc_name);
}

void DesignExtractor::Visit(std::shared_ptr<StatementNode> node, std::string proc_name) {
  // runtime polymorphism decides which specific StatementNode's Accept method to invoke
  node->Accept(this, proc_name);
}

std::string DesignExtractor::Visit(std::shared_ptr<ExpressionNode> node, std::string proc_name, bool is_uses) {
  // runtime polymorphism decides which specific ExpressionNode's Accept method to invoke
  return node->Accept(this, proc_name, is_uses);
}

std::string DesignExtractor::Visit(std::shared_ptr<ConditionalExpressionNode> node,
                                   std::string proc_name,
                                   bool is_uses) {
  // runtime polymorphism decides which specific ConditionalExpressionNode's Accept method to invoke
  return node->Accept(this, proc_name, is_uses);
}

void DesignExtractor::Visit(std::shared_ptr<VariableNode> node, std::string proc_name, bool is_uses) {
  node->Accept(this, proc_name, is_uses);
}

void DesignExtractor::Visit(std::shared_ptr<ConstantNode> node, std::string proc_name, bool is_uses) {
  // ignores is_uses
  node->Accept(this, proc_name, is_uses);
}
