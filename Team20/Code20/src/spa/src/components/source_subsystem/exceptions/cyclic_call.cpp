#include "cyclic_call.h"

namespace source {

CyclicCallException::CyclicCallException() : m_msg("Detected cyclic procedure calls.") {}

const char *CyclicCallException::what() const noexcept {
  return m_msg.c_str();
}

}
