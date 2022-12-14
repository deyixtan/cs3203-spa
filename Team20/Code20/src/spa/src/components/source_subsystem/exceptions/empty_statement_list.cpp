#include "empty_statement_list.h"

namespace source {

EmptyStatementListException::EmptyStatementListException() : m_msg("Statement List cannot be empty.") {}

const char *EmptyStatementListException::what() const noexcept {
  return m_msg.c_str();
}

}
