#ifndef SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_PROGRAM_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_PROGRAM_H_

#include <vector>

#include "node.h"
#include "node_procedure.h"

class ProgramNode : public Node
{
 private:
  std::vector<std::shared_ptr<ProcedureNode>> m_procedures;

 public:
  ProgramNode(std::vector<std::shared_ptr<ProcedureNode>> procedures);
  std::vector<std::shared_ptr<ProcedureNode>> getProcedures();
  std::string ToString();
  std::string ToString(int level);
};

#endif //SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_PROGRAM_H_
