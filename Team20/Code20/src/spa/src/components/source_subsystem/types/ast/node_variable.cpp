#include "node_variable.h"
#include "../../iterator/design_extractor.h"

VariableNode::VariableNode() : m_identifier("") {}

VariableNode::VariableNode(std::string identifier, std::string stmt, std::string proc) : m_identifier(identifier), m_stmt(stmt), m_proc(proc) {}

std::string VariableNode::GetIdentifier() {
  return m_identifier;
}

ExpressionType VariableNode::GetExpressionType() {
  return ExpressionType::VARIABLE;
}

std::string VariableNode::ToString() {
  return "$" + m_identifier;
}

std::string VariableNode::GetPatternFormat() {
  return "(" + m_identifier + ")";
}

bool VariableNode::operator==(const ExpressionNode &other) const {
  const auto casted_other = dynamic_cast<const VariableNode *>(&other);
  return m_identifier == casted_other->m_identifier;
}

std::string VariableNode::Accept(DesignExtractor *de, bool is_uses) {
  std::string var_name = m_identifier;
  if (!is_uses) {
    for (std::string s : de->GetVisited()) {
      de->GetPopulator()->PopulateModifies(s, var_name);
    }
    de->GetPopulator()->PopulateModifiesProc(m_proc, var_name);
    de->GetPopulator()->PopulateModifies(m_stmt, var_name);
  }

  if (is_uses) {
    for (std::string s : de->GetVisited()) {
      de->GetPopulator()->PopulateUses(s, var_name);
    }
    de->GetPopulator()->PopulateUsesProc(m_proc, var_name);
    de->GetPopulator()->PopulateUses(m_stmt, var_name);
  }

  de->GetPopulator()->PopulateVars(var_name);
  return GetPatternFormat();
}