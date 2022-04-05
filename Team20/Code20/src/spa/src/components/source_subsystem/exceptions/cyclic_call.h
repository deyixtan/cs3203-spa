#ifndef CYCLIC_CALL_H_
#define CYCLIC_CALL_H_

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

#endif //CYCLIC_CALL_H_
