#include "invalid_parse_condition.h"

namespace source {

std::string InvalidParseConditionException::MESSAGE = "Unable to parse conditional expression.";

InvalidParseConditionException::InvalidParseConditionException() : m_msg(MESSAGE) {}

const char *InvalidParseConditionException::what() const noexcept {
  return m_msg.c_str();
}

}
