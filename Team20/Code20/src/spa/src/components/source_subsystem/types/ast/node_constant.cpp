#include "node_constant.h"
#include "components/source_subsystem/pkb_client.h"
#include "components/source_subsystem/iterator/design_extractor.h"

namespace source {

ConstantNode::ConstantNode(String value) : m_value(std::move(value)) {}

String ConstantNode::GetValue() {
  return m_value;
}

String ConstantNode::GetPatternFormat() {
  return "(" + m_value + ")";
}

String ConstantNode::Accept(DesignExtractor *design_extractor, String proc_name, bool is_uses) {
  design_extractor->GetPkbClient()->PopulateConst(m_value);
  return GetPatternFormat();
}

bool ConstantNode::operator==(const ExpressionNode &other) const {
  const auto casted_other = dynamic_cast<const ConstantNode *>(&other);
  return m_value == casted_other->m_value;
}

}
