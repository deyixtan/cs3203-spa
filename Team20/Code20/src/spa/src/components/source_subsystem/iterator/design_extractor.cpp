#include "design_extractor.h"
#include "../types/call_graph/call_graph.h"
#include "../types/ast/node_program.h"
#include "../types/ast/node_procedure.h"
#include "../types/ast/node_statement_list.h"
#include "../types/ast/node_statement.h"
#include "../types/ast/node_conditional_expression.h"
#include "../types/ast/node_variable.h"
#include "../types/ast/node_constant.h"
#include "components/source_subsystem/pkb_client.h"

namespace source {

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
  std::unordered_set<std::string> uses_vars = m_pkb_client->GetPKB()->GetUsesStore()->GetVarUsedByStmt(proc);
  std::unordered_set<std::string> mod_vars = m_pkb_client->GetPKB()->GetModifiesStore()->GetVarModByStmt(proc);
  std::unordered_set<std::string> call_stmts = m_pkb_client->GetPKB()->GetCallStore()->GetCallStmtOf(proc);
  std::unordered_set<std::string> callers = m_pkb_client->GetPKB()->GetCallStore()->GetCallersOf(proc);

  for (auto &call_stmt : call_stmts) {
    std::unordered_set<std::string> ancestors = m_pkb_client->GetPKB()->GetParentStore()->GetAllAnceOf(STMT, call_stmt); //TODO: Fix StmtType

    UpdateCallUses(call_stmt, uses_vars, ancestors, callers);
    UpdateCallModifies(call_stmt, mod_vars, ancestors, callers);
  }
}

void DesignExtractor::UpdateCallUses(std::string const &call_stmt,
                                     std::unordered_set<std::string> const &vars,
                                     std::unordered_set<std::string> const &ancestors,
                                     std::unordered_set<std::string> const &callers) {
  for (auto &var : vars) {
    m_pkb_client->GetPKB()->GetUsesStore()->AddStmtVar(call_stmt, var);
    for (auto &ance : ancestors) {
      m_pkb_client->GetPKB()->GetUsesStore()->AddStmtVar(ance, var);
    }
    for (auto &caller : callers) {
      m_pkb_client->GetPKB()->GetUsesStore()->AddStmtVar(caller, var);
    }
  }
}

void DesignExtractor::UpdateCallModifies(std::string const &call_stmt,
                                         std::unordered_set<std::string> const &vars,
                                         std::unordered_set<std::string> const &ancestors,
                                         std::unordered_set<std::string> const &callers) {
  for (auto &var : vars) {
    m_pkb_client->GetPKB()->GetModifiesStore()->AddStmtVar(call_stmt, var);
    for (auto &ance : ancestors) {
      m_pkb_client->GetPKB()->GetModifiesStore()->AddStmtVar(ance, var);
    }
    for (auto &caller : callers) {
      m_pkb_client->GetPKB()->GetModifiesStore()->AddStmtVar(caller, var);
    }

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

}
