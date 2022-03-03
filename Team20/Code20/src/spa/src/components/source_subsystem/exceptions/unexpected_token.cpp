#include "unexpected_token.h"

namespace source {

std::string UnexpectedTokenException::MESSAGE = "Unexpected token.";

UnexpectedTokenException::UnexpectedTokenException() : m_msg(MESSAGE) {}

const char *UnexpectedTokenException::what() const noexcept {
  return m_msg.c_str();
}

}
