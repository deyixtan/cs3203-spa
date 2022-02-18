#include "invalid_parse_statement.h"

namespace source {

std::string InvalidParseStatementException::MESSAGE = "Unable to parse statement.";

InvalidParseStatementException::InvalidParseStatementException() : m_msg(MESSAGE) {}

const char *InvalidParseStatementException::what() const noexcept {
  return m_msg.c_str();
}

}
