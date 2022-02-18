#include "invalid_parse_factor.h"

namespace source {

std::string InvalidParseFactorException::MESSAGE = "Unable to parse factor.";

InvalidParseFactorException::InvalidParseFactorException() : m_msg(MESSAGE) {}

const char *InvalidParseFactorException::what() const noexcept {
  return m_msg.c_str();
}

}
