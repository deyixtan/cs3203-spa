#include "node_program.h"

ProgramNode::ProgramNode(std::vector<std::shared_ptr<ProcedureNode>> procedures) :
    m_procedures(procedures) {}

std::vector<std::shared_ptr<ProcedureNode>> ProgramNode::getProcedures() {
  return m_procedures;
}

std::string ProgramNode::ToString() {
  return this->ToString(0);
}

std::string ProgramNode::ToString(int level) {
  std::string acc = "";
  for (std::shared_ptr<ProcedureNode> procedure : m_procedures) {
    acc += procedure->ToString(level) + "\n";
  }
  return acc;
}
