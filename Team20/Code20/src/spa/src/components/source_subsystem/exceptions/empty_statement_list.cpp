#include "empty_statement_list.h"

namespace source {

std::string EmptyStatementListException::MESSAGE = "Statement List cannot be empty.";

EmptyStatementListException::EmptyStatementListException() : m_msg(MESSAGE) {}

const char *EmptyStatementListException::what() const noexcept {
  return m_msg.c_str();
}

}
