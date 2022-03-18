#include <queue>
#include <iostream>
#include "design_extractor.h"
#include "../types/ast/node_program.h"
#include "../types/ast/node_procedure.h"
#include "../types/ast/node_statement_list.h"
#include "../types/ast/node_statement.h"
#include "../types/ast/node_conditional_expression.h"
#include "../types/ast/node_variable.h"
#include "../types/cfg/cfg_node.h"

DesignExtractor::DesignExtractor(std::shared_ptr<PkbClient> pkb_client)
    : m_pkb_client(std::move(pkb_client)), m_visited(std::vector<std::string>()) {}

std::shared_ptr<PkbClient> DesignExtractor::GetPkbClient() {
  return m_pkb_client;
}

std::vector<std::string> &DesignExtractor::GetVisited() {
  return m_visited;
}

void DesignExtractor::IterateAstAndPopulatePkb(std::shared_ptr<ProgramNode> node) {
  Visit(std::move(node));
}

void DesignExtractor::IterateCfgAndPopulatePkb(std::shared_ptr<Cfg> root) {
  std::queue<std::shared_ptr<CfgNode>> node_queue;
  std::vector<std::string> prev_stmts;
  std::unordered_map<std::string, std::unordered_set<std::string>> next_map;
  std::unordered_map<std::string, std::shared_ptr<CfgNode>> prog = root->GetCfgMap();
  for (auto proc : prog) {
    std::shared_ptr<CfgNode> curr_proc = proc.second;
    node_queue.push(curr_proc);
    while(node_queue.size() > 0) {
      std::shared_ptr<CfgNode> curr = node_queue.front();
      node_queue.pop();
      std::vector<std::string> curr_stmts = curr->GetStatementList();
      std::vector<std::shared_ptr<CfgNode>> next_nodes = curr->GetDescendants();
      if(curr_stmts.size() > 0 && curr_stmts.front() == "") {
        break;
      }
      int start = 0;
      int next = 1;
      while(next < curr_stmts.size()) {
        if(next_map.find(curr_stmts[start]) == next_map.end()) {
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
      for(auto const &desc : next_nodes) {
        std::vector<std::string> next_stmts = desc->GetStatementList();
        std::unordered_set<std::string> vals = next_map[curr_stmts[curr_stmts.size() - 1]];
        if(next_stmts.front() != "") {
          vals.insert(next_stmts.front());
        }
        node_queue.push(desc);
      }
    }
  }
  m_pkb_client->PopulateNext(next_map);
}

void DesignExtractor::Visit(std::shared_ptr<ProgramNode> node) {
  node->Accept(this);
}

void DesignExtractor::Visit(std::shared_ptr<ProcedureNode> node) {
  node->Accept(this);
}

void DesignExtractor::Visit(std::shared_ptr<StatementListNode> node) {
  node->Accept(this);
}

void DesignExtractor::Visit(std::shared_ptr<StatementNode> node) {
  // runtime polymorphism decides which specific StatementNode's Accept method to invoke
  node->Accept(this);
}

std::string DesignExtractor::Visit(std::shared_ptr<ExpressionNode> node, bool is_uses) {
  // runtime polymorphism decides which specific ExpressionNode's Accept method to invoke
  return node->Accept(this, is_uses);
}

std::string DesignExtractor::Visit(std::shared_ptr<ConditionalExpressionNode> node, bool is_uses) {
  // runtime polymorphism decides which specific ConditionalExpressionNode's Accept method to invoke
  return node->Accept(this, is_uses);
}

void DesignExtractor::Visit(std::shared_ptr<VariableNode> node, bool is_uses) {
  node->Accept(this, is_uses);
}
