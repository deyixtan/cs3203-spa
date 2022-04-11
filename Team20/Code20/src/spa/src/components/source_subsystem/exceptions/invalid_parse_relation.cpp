#include "invalid_parse_relation.h"

namespace source {

InvalidParseRelationException::InvalidParseRelationException() : m_msg("Unable to parse relational expression.") {}

const char *InvalidParseRelationException::what() const noexcept {
  return m_msg.c_str();
}

}
