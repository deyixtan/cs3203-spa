#include "node_program.h"
#include "components/source_subsystem/iterator/cfg_builder.h"
#include "components/source_subsystem/iterator/design_extractor.h"
#include "components/source_subsystem/types/ast/node_procedure.h"

namespace source {

ProgramNode::ProgramNode() = default;

ProgramNode::ProgramNode(ProcedureNodeStream procedures) :
    m_procedures(std::move(procedures)) {}

void ProgramNode::Accept(DesignExtractor *design_extractor) {
  for (auto &procedure : m_procedures) {
    design_extractor->Visit(procedure);
  }
}

StringToCfgNodePtrMap ProgramNode::Accept(CfgBuilder *cfg_builder) {
  StringToCfgNodePtrMap heads;
  for (ProcedureNodePtr const &procedure : m_procedures) {
    CfgNodePtr head = std::make_shared<CfgNode>();
    CfgNodePtr tail = cfg_builder->Visit(procedure, head);
    if (!tail->GetStatementList().empty()) {
      CfgNodePtr dummy = std::make_shared<CfgNode>();
      tail->AddNext(dummy);
    }
    heads.insert({procedure->GetName(), head});
  }
  return heads;
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
