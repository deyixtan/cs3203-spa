#include "node_program.h"
#include "node_procedure.h"
#include "../cfg/cfg.h"
#include "../cfg/cfg_node.h"
#include "../../iterator/design_extractor.h"
#include "../../iterator/cfg_builder.h"

namespace source {

ProgramNode::ProgramNode() {}

ProgramNode::ProgramNode(ProcedureNodeStream procedures) :
    m_procedures(procedures) {}

String ProgramNode::GetPatternFormat() {
  return "";
}

void ProgramNode::Accept(DesignExtractor *de) {
  for (auto &procedure : m_procedures) {
    de->Visit(procedure);
  }
}

StringToCfgNodePtrMap ProgramNode::Accept(CfgBuilder *cb) {
  StringToCfgNodePtrMap heads;
  for (ProcedureNodePtr const &procedure : m_procedures) {
    CfgNodePtr head = std::make_shared<CfgNode>();
    CfgNodePtr tail = cb->Visit(procedure, head);
    if (tail->GetStatementList().size() != 0) {
      CfgNodePtr dummy = std::make_shared<CfgNode>();
      tail->AddNext(dummy);
    }
    heads.insert({procedure->GetIdentifier(), head});
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
