#include "invalid_parse.h"

namespace source {

InvalidParseException::InvalidParseException(std::string msg) : m_msg(msg) {}

const char *InvalidParseException::what() const noexcept {
  return m_msg.c_str();
}

}
