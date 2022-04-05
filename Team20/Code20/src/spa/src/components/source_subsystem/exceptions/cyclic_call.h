#ifndef CYCLIC_CALL_H
#define CYCLIC_CALL_H

#include "components/source_subsystem/source_declarations.h"

namespace source {

class CyclicCallException : public Exception {
 private:
  String m_msg;

 public:
  CyclicCallException();
  [[nodiscard]] const char *what() const noexcept override;
};

}

#endif //CYCLIC_CALL_H
