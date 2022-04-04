#include "cfg_builder.h"
#include "components/source_subsystem/pkb_client.h"
#include "../types/ast/node_program.h"
#include "../types/ast/node_procedure.h"
#include "../types/ast/node_statement_list.h"
#include "../types/ast/node_statement.h"
#include "../types/cfg/cfg.h"
#include "../types/cfg/cfg_node.h"

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
