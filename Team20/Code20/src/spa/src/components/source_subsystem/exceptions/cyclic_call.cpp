#include "cyclic_call.h"

namespace source {

String CyclicCallException::MESSAGE = "Detected cyclic procedure calls.";

CyclicCallException::CyclicCallException() : m_msg(MESSAGE) {}

const char *CyclicCallException::what() const noexcept {
  return m_msg.c_str();
}

}
