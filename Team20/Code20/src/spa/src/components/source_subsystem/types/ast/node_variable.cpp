#include "node_variable.h"
#include "components/source_subsystem/iterator/design_extractor.h"

namespace source {

VariableNode::VariableNode(String name) : m_name(std::move(name)) {}

String VariableNode::GetName() {
  return m_name;
}

String VariableNode::GetPatternFormat() {
  String name = m_name;
  return "(" + name + ")";
}

void VariableNode::Accept(DesignExtractorPtr design_extractor) {
  VariableNodePtr derived_ptr = std::dynamic_pointer_cast<VariableNode>(shared_from_this());
  design_extractor->Visit(derived_ptr);
}

void VariableNode::Accept(CfgBuilderPtr cfg_builder) {}

bool VariableNode::operator==(const ExpressionNode &other) const {
  const auto casted_other = dynamic_cast<const VariableNode *>(&other);
  return m_name == casted_other->m_name;
}

}
