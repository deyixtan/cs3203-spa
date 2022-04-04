#include "invalid_term_operator.h"

namespace source {

std::string InvalidTermOperatorException::MESSAGE = "Invalid term operator. Expected '*', '/' or '%' operator.";

InvalidTermOperatorException::InvalidTermOperatorException() : m_msg(MESSAGE) {}

const char *InvalidTermOperatorException::what() const noexcept {
  return m_msg.c_str();
}

}
