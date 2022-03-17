#include "cfg_builder.h"
#include "components/source_subsystem/types/ast/node_program.h"
#include "components/source_subsystem/types/ast/node_procedure.h"
#include "components/source_subsystem/types/ast/node_statement_list.h"
#include "components/source_subsystem/types/ast/node_statement.h"

CfgBuilder::CfgBuilder(Populator *populator)
    : m_populator(populator),
      m_visited(std::vector<std::string>()) {}

Populator *CfgBuilder::GetPopulator() {
  return m_populator;
}

std::vector<std::string> &CfgBuilder::GetVisited() {
  return m_visited;
}

void CfgBuilder::Visit(std::shared_ptr<ProgramNode> node) {
  node->Accept(this);
}

std::shared_ptr<CfgNode> CfgBuilder::Visit(std::shared_ptr<ProcedureNode> node, std::shared_ptr<CfgNode> cfg_node) {
  return node->Accept(this, cfg_node);
}

std::shared_ptr<CfgNode> CfgBuilder::Visit(std::shared_ptr<StatementListNode> node, std::shared_ptr<CfgNode> cfg_node) {
  return node->Accept(this, cfg_node);
}

std::shared_ptr<CfgNode> CfgBuilder::Visit(std::shared_ptr<StatementNode> node, std::shared_ptr<CfgNode> cfg_node) {
  // runtime polymorphism decides which specific StatementNode's Accept method to invoke
  return node->Accept(this, cfg_node);
}
