#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_PROGRAM_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_PROGRAM_H_

#include <vector>

#include "node.h"
#include "node_procedure.h"

class ProgramNode : public Node {
 private:
  std::vector<std::shared_ptr<ProcedureNode>> m_procedures;

 public:
  ProgramNode();
  ProgramNode(std::vector<std::shared_ptr<ProcedureNode>> procedures);
  [[nodiscard]] std::vector<std::shared_ptr<ProcedureNode>> GetProcedures();
  [[nodiscard]] std::string Process(Populator populator, std::vector<std::string> *visited, bool is_uses, std::shared_ptr<source::CfgProcedureNode> cfg_proc_node, std::shared_ptr<source::CfgGroupNode> cfg_node) override;
  [[nodiscard]] std::string ToString() override;
  [[nodiscard]] std::string GetPatternFormat() override;
  [[nodiscard]] bool operator==(const ProgramNode &other) const;
};

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_PROGRAM_H_
