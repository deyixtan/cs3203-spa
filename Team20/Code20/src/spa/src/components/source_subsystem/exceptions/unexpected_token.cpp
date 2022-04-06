#include "unexpected_token.h"

namespace source {

UnexpectedTokenException::UnexpectedTokenException() : m_msg("Unexpected token.") {}

const char *UnexpectedTokenException::what() const noexcept {
  return m_msg.c_str();
}

}
