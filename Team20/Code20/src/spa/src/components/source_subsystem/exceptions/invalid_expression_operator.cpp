#include "invalid_expression_operator.h"

namespace source {

std::string InvalidExpressionOperatorException::MESSAGE = "Invalid expression operator. Expected '+' or '-' operator.";

InvalidExpressionOperatorException::InvalidExpressionOperatorException() : m_msg(MESSAGE) {}

const char *InvalidExpressionOperatorException::what() const noexcept {
  return m_msg.c_str();
}

}
