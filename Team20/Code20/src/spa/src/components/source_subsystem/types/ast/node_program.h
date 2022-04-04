#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_PROGRAM_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_PROGRAM_H_

#include "node.h"

namespace source {

class ProgramNode : public Node {
 private:
  std::vector<std::shared_ptr<ProcedureNode>> m_procedures;

 public:
  ProgramNode();
  ProgramNode(std::vector<std::shared_ptr<ProcedureNode>> procedures);
  [[nodiscard]] std::string ToString() override;
  [[nodiscard]] std::string GetPatternFormat() override;
  [[nodiscard]] bool operator==(const ProgramNode &other) const;
  void Accept(DesignExtractor *de);
  std::unordered_map<std::string, std::shared_ptr<CfgNode>> Accept(CfgBuilder *cb);
};

}

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_AST_NODE_PROGRAM_H_
