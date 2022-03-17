#ifndef SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_ITERATOR_CFG_BUILDER_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_ITERATOR_CFG_BUILDER_H_

#include <string>
#include <unordered_map>
#include <vector>

class Populator;
class ProgramNode;
class ProcedureNode;
class StatementListNode;
class StatementNode;
class CfgNode;

class CfgBuilder {
 private:
  Populator *m_populator;
  std::vector<std::string> m_visited;
  std::string m_curr_proc;

 public:
  CfgBuilder(Populator *populator);
  [[nodiscard]] Populator *GetPopulator();
  [[nodiscard]] std::vector<std::string> &GetVisited();
  void Visit(std::shared_ptr<ProgramNode> node);
  std::shared_ptr<CfgNode> Visit(std::shared_ptr<ProcedureNode> node, std::shared_ptr<CfgNode> cfg_node);
  std::shared_ptr<CfgNode> Visit(std::shared_ptr<StatementListNode> node, std::shared_ptr<CfgNode> cfg_node);
  std::shared_ptr<CfgNode> Visit(std::shared_ptr<StatementNode> node, std::shared_ptr<CfgNode> cfg_node);
};

#endif //SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_ITERATOR_CFG_BUILDER_H_
