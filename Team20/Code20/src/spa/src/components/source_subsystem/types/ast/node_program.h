#ifndef NODE_PROGRAM_H
#define NODE_PROGRAM_H

#include "components/source_subsystem/source_declarations.h"
#include "components/source_subsystem/types/ast/node.h"

namespace source {

class ProgramNode : public Node {
 private:
  ProcedureNodeStream m_procedures;

 public:
  ProgramNode();
  explicit ProgramNode(ProcedureNodeStream procedures);
  [[nodiscard]] ProcedureNodeStream GetProcedures();
  void Accept(DesignExtractorPtr design_extractor) override;
  CfgNodePtr Accept(CfgBuilderPtr cfg_builder, CfgNodePtr cfg_node) override;
  [[nodiscard]] bool operator==(const ProgramNode &other) const;
};

}

#endif //NODE_PROGRAM_H
