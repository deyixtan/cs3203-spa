#include "invalid_term_operator.h"

namespace source {

InvalidTermOperatorException::InvalidTermOperatorException() :
    m_msg("Invalid term operator. Expected '*', '/' or '%' operator.") {}

const char *InvalidTermOperatorException::what() const noexcept {
  return m_msg.c_str();
}

}
s