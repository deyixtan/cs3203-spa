#include "invalid_call.h"

namespace source {

std::string InvalidCallException::MESSAGE = "Invoked invalid method.";

InvalidCallException::InvalidCallException() : m_msg(MESSAGE) {}

const char *InvalidCallException::what() const noexcept {
  return m_msg.c_str();
}

}
