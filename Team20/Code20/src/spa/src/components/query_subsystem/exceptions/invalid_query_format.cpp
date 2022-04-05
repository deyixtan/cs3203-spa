#include "invalid_query_format.h"

InvalidQueryFormatException::InvalidQueryFormatException() : m_msg("Invalid query format.") {}

const char *InvalidQueryFormatException::what() const noexcept {
  return m_msg.c_str();
}