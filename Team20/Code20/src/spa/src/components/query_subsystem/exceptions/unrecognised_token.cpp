#include "unrecognised_token.h"

UnrecognisedTokenException::UnrecognisedTokenException() : m_msg("Unrecognised token.") {}

const char *UnrecognisedTokenException::what() const noexcept {
  return m_msg.c_str();
}
