#ifndef MISMATCH_TOKEN_H
#define MISMATCH_TOKEN_H

#include "components/source_subsystem/source_declarations.h"

namespace source {

class MismatchedTokenException : public Exception {
 private:
  String m_msg;

 public:
  MismatchedTokenException();
  [[nodiscard]] const char *what() const noexcept override;
};

}

#endif //MISMATCH_TOKEN_H
