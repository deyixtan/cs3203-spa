#include "invalid_parse_factor.h"

namespace source {

InvalidParseFactorException::InvalidParseFactorException() : m_msg("Unable to parse factor.") {}

const char *InvalidParseFactorException::what() const noexcept {
  return m_msg.c_str();
}

}
