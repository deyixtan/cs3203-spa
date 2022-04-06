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
  void Accept(DesignExtractorPtr design_extractor) override;
  //void Accept(DesignExtractor *design_extractor);
  StringToCfgNodePtrMap Accept(CfgBuilder *cfg_builder);
  [[nodiscard]] bool operator==(const ProgramNode &other) const;
};

}

#endif //NODE_PROGRAM_H
