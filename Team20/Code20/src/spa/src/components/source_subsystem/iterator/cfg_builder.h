#ifndef SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_ITERATOR_CFG_BUILDER_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_ITERATOR_CFG_BUILDER_H_

#include <memory>
#include <string>
#include <unordered_map>

class PkbClient;
class ProgramNode;
class ProcedureNode;
class StatementListNode;
class StatementNode;
class Cfg;
class CfgNode;

class CfgBuilder {
 private:
  PkbClient *m_pkb_client;

 public:
  CfgBuilder(PkbClient *m_pkb_client);
  [[nodiscard]] PkbClient *GetPkbClient();
  void IterateAstAndPopulateCfg(std::shared_ptr<ProgramNode> node);
  [[nodiscard]] std::unordered_map<std::string, std::shared_ptr<CfgNode>> Visit(std::shared_ptr<ProgramNode> node);
  [[nodiscard]] std::shared_ptr<CfgNode> Visit(std::shared_ptr<ProcedureNode> node, std::shared_ptr<CfgNode> cfg_node);
  [[nodiscard]] std::shared_ptr<CfgNode> Visit(std::shared_ptr<StatementListNode> node, std::shared_ptr<CfgNode> cfg_node);
  [[nodiscard]] std::shared_ptr<CfgNode> Visit(std::shared_ptr<StatementNode> node, std::shared_ptr<CfgNode> cfg_node);
};

#endif //SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_ITERATOR_CFG_BUILDER_H_
