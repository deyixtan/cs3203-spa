#include "design_extractor.h"
#include "../types/ast/node_program.h"
#include "../types/ast/node_procedure.h"
#include "../types/ast/node_statement_list.h"
#include "../types/ast/node_statement.h"
#include "../types/ast/node_conditional_expression.h"
#include "../types/ast/node_variable.h"
#include "../types/ast/node_constant.h"

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

std::string DesignExtractor::Visit(std::shared_ptr<ConditionalExpressionNode> node, std::string proc_name, bool is_uses) {
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
