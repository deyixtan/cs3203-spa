#include "end_of_stream.h"

namespace source {

std::string EndOfStreamException::MESSAGE = "Reached end of stream.";

EndOfStreamException::EndOfStreamException() : m_msg(MESSAGE) {}

const char *EndOfStreamException::what() const noexcept {
  return m_msg.c_str();
}

}
