#include "invalid_call.h"

namespace source {

InvalidCallException::InvalidCallException() : m_msg("Called invalid method.") {}

const char *InvalidCallException::what() const noexcept {
  return m_msg.c_str();
}

}
