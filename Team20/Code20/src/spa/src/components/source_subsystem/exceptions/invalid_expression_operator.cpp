#include "invalid_expression_operator.h"

namespace source {

InvalidExpressionOperatorException::InvalidExpressionOperatorException() :
    m_msg("Invalid expression operator. Expected '+' or '-' operator.") {}

const char *InvalidExpressionOperatorException::what() const noexcept {
  return m_msg.c_str();
}

}
