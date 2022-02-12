#include "node_program.h"

ProgramNode::ProgramNode() {}

ProgramNode::ProgramNode(std::vector<std::shared_ptr<ProcedureNode>> procedures) :
    m_procedures(procedures) {}

std::vector<std::shared_ptr<ProcedureNode>> ProgramNode::GetProcedures() {
  return m_procedures;
}

std::string ProgramNode::ToString(int level) {
  std::string str = "";
  for (std::shared_ptr<ProcedureNode> procedure : m_procedures) {
    str += procedure->ToString(level) + "\n";
  }
  return str;
}

std::string ProgramNode::ToString() {
  return this->ToString(0);
}
