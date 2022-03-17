#include "node_constant.h"
#include "../../iterator/design_extractor.h"

ConstantNode::ConstantNode(std::string value) : m_value(value) {}

std::string ConstantNode::GetValue() {
  return m_value;
}

ExpressionType ConstantNode::GetExpressionType() {
  return ExpressionType::CONSTANT;
}

std::string ConstantNode::ToString() {
  return m_value;
}

std::string ConstantNode::Process(Populator populator, std::vector<std::string> *visited, bool is_uses, std::shared_ptr<source::CfgProcedureNode> cfg_proc_node, std::shared_ptr<source::CfgGroupNode> cfg_node) {
  populator.PopulateConst(m_value);
  return GetPatternFormat();
}

std::string ConstantNode::GetPatternFormat() {
  return "(" + m_value + ")";
}

bool ConstantNode::operator==(const ExpressionNode &other) const {
  const auto casted_other = dynamic_cast<const ConstantNode *>(&other);
  return m_value == casted_other->m_value;
}

std::string ConstantNode::Accept(DesignExtractor *de, bool is_uses) {
  de->GetPopulator()->PopulateConst(m_value);
  return GetPatternFormat();
}