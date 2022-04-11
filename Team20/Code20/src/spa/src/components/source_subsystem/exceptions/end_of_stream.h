#ifndef END_OF_STREAM_H
#define END_OF_STREAM_H

#include "components/source_subsystem/source_declarations.h"

namespace source {

class EndOfStreamException : public Exception {
 private:
  String m_msg;

 public:
  EndOfStreamException();
  [[nodiscard]] const char *what() const noexcept override;
};

}

#endif //END_OF_STREAM_H
