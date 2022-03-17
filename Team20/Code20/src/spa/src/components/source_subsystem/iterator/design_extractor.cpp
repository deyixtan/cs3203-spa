#include "design_extractor.h"
#include "components/source_subsystem/types/ast/node_program.h"
#include "components/source_subsystem/types/ast/node_procedure.h"
#include "components/source_subsystem/types/ast/node_statement_list.h"
#include "components/source_subsystem/types/ast/node_statement.h"
#include "components/source_subsystem/types/ast/node_conditional_expression.h"
#include "components/source_subsystem/types/ast/node_variable.h"

DesignExtractor::DesignExtractor(PkbClient *pkb_client)
    : m_pkb_client(pkb_client), m_visited(std::vector<std::string>()) {}

PkbClient *DesignExtractor::GetPkbClient() {
  return m_pkb_client;
}

std::vector<std::string> &DesignExtractor::GetVisited() {
  return m_visited;
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
