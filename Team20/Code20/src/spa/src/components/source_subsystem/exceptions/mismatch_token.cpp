#include "mismatch_token.h"

namespace source {

MismatchedTokenException::MismatchedTokenException() : m_msg("Mismatched token type.") {}

const char *MismatchedTokenException::what() const noexcept {
  return m_msg.c_str();
}

}
