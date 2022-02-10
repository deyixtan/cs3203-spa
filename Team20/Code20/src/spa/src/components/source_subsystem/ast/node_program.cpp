#include "node_program.h"

ProgramNode::ProgramNode(std::vector<std::shared_ptr<ProcedureNode>> procedures) :
    m_procedures(procedures) {}

std::vector<std::shared_ptr<ProcedureNode>> ProgramNode::getProcedures() {
  return m_procedures;
}

std::string ProgramNode::format() {
  return this->format(0);
}

std::string ProgramNode::format(int level) {
  std::string acc = "";
  for (std::shared_ptr<ProcedureNode> procedure : m_procedures) {
    acc += procedure->format(level) + "\n";
  }
  return acc;
}
