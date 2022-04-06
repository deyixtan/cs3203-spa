#include "node_program.h"
#include "components/source_subsystem/iterator/cfg_builder.h"
#include "components/source_subsystem/iterator/design_extractor.h"
#include "components/source_subsystem/types/ast/node_procedure.h"

namespace source {

ProgramNode::ProgramNode() = default;

ProgramNode::ProgramNode(ProcedureNodeStream procedures) :
    m_procedures(std::move(procedures)) {}

ProcedureNodeStream ProgramNode::GetProcedures() {
  return m_procedures;
}

void ProgramNode::Accept(DesignExtractorPtr design_extractor) {
  ProgramNodePtr derived_ptr = std::dynamic_pointer_cast<ProgramNode>(shared_from_this());
  design_extractor->Visit(derived_ptr);
}

CfgNodePtr ProgramNode::Accept(CfgBuilderPtr cfg_builder, CfgNodePtr cfg_node) {
  ProgramNodePtr derived_ptr = std::dynamic_pointer_cast<ProgramNode>(shared_from_this());
  return cfg_builder->Visit(derived_ptr, cfg_node);
}

bool ProgramNode::operator==(const ProgramNode &other) const {
  if (m_procedures.size() != other.m_procedures.size()) {
    return false;
  }
  for (int i = 0; i < m_procedures.size(); i++) {
    if (*(m_procedures.at(i)) == *(other.m_procedures.at(i))) {}
    else { return false; }
  }
  return true;
}

}
