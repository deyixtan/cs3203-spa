#include "invalid_parse_relation.h"

namespace source {

std::string InvalidParseRelationException::MESSAGE = "Unable to parse relational expression.";

InvalidParseRelationException::InvalidParseRelationException() : m_msg(MESSAGE) {}

const char *InvalidParseRelationException::what() const noexcept {
  return m_msg.c_str();
}

}
