#include "node_constant.h"
#include "components/source_subsystem/iterator/design_extractor.h"

namespace source {

ConstantNode::ConstantNode(String value) : m_value(std::move(value)) {}

String ConstantNode::GetValue() {
  return m_value;
}

String ConstantNode::GetPatternFormat() {
  String value = m_value;
  return "(" + value + ")";
}

void ConstantNode::Accept(DesignExtractorPtr design_extractor) {
  ConstantNodePtr derived_ptr = std::dynamic_pointer_cast<ConstantNode>(shared_from_this());
  design_extractor->Visit(derived_ptr);
}

void ConstantNode::Accept(CfgBuilderPtr cfg_builder) {}

bool ConstantNode::operator==(const ExpressionNode &other) const {
  const auto casted_other = dynamic_cast<const ConstantNode *>(&other);
  return m_value == casted_other->m_value;
}

}
