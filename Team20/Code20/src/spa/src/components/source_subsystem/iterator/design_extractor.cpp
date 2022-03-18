#include <queue>
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
  std::queue<std::shared_ptr<CfgNode>> nodeQueue;
  std::vector<std::string> prevStmts;
  std::unordered_map<std::string, std::unordered_set<std::string>> rs_map;
  for (auto const& proc : root->GetCfgMap()) {
    std::shared_ptr<CfgNode> currProc = proc.second;
    nodeQueue.push(currProc);
    while(nodeQueue.size() > 0) {
      std::shared_ptr<CfgNode> curr = nodeQueue.front();
      std::vector<std::string> currStmts = curr->GetStatementList();
      if(currStmts.front() == "") {
        break;
      }
      int start = 0;
      int next = 1;
      while(next < currStmts.size()) {
        if(rs_map.find(currStmts[start]) == rs_map.end()) {
          rs_map.insert(currStmts[start], currStmts[next]);
        }
        start++;
        next++;
      }
      std::vector<std::shared_ptr<CfgNode>> next_nodes = curr->GetDescendants();
      for(auto desc : next_nodes) {
        std::vector<std::string> nextStmts = desc->GetStatementList();
        std::unordered_set<std::string> vals = rs_map[currStmts[currStmts.size() - 1]];
        if(nextStmts.front() != "") {
          vals.insert(nextStmts.front());
        }
        nodeQueue.push(desc);
      }
    }
  }
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
