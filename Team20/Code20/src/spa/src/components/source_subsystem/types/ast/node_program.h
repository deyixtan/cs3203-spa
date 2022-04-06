#ifndef NODE_PROGRAM_H
#define NODE_PROGRAM_H

#include "components/source_subsystem/source_declarations.h"
#include "components/source_subsystem/types/ast/node.h"

namespace source {

class ProgramNode : public Node, std::enable_shared_from_this<ProgramNode> {
 private:
  ProcedureNodeStream m_procedures;

 public:
  ProgramNode();
  explicit ProgramNode(ProcedureNodeStream procedures);
  [[nodiscard]] ProcedureNodeStream GetProcedures();
  void Accept(DesignExtractorPtr design_extractor) override;
  StringToCfgNodePtrMap Accept(CfgBuilder *cfg_builder);
  [[nodiscard]] bool operator==(const ProgramNode &other) const;
};

}

#endif //NODE_PROGRAM_H
