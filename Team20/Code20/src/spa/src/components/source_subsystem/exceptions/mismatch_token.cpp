#include "mismatch_token.h"

namespace source {

std::string MismatchedTokenException::MESSAGE = "Mismatched token type.";

MismatchedTokenException::MismatchedTokenException() : m_msg(MESSAGE) {}

const char *MismatchedTokenException::what() const noexcept {
  return m_msg.c_str();
}

}
