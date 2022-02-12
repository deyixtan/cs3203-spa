#include "end_of_stream.h"

namespace source {

EndOfStreamException::EndOfStreamException() : m_msg("Already reached end of stream.") {}

const char *EndOfStreamException::what() const noexcept {
  return m_msg.c_str();
}

}
