#include "invalid_parse_condition.h"

namespace source {

InvalidParseConditionException::InvalidParseConditionException() : m_msg("Unable to parse conditional expression.") {}

const char *InvalidParseConditionException::what() const noexcept {
  return m_msg.c_str();
}

}
