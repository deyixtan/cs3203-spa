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
  [[nodiscard]] std::string ToString(int level) override;
  [[nodiscard]] std::string ToString();
};

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_PROGRAM_H_