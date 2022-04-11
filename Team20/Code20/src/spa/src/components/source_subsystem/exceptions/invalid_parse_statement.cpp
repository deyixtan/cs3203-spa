#include "invalid_parse_statement.h"

namespace source {

InvalidParseStatementException::InvalidParseStatementException() : m_msg("Unable to parse statement.") {}

const char *InvalidParseStatementException::what() const noexcept {
  return m_msg.c_str();
}

}
