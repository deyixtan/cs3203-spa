#ifndef PROCEDURE_EXIST_H
#define PROCEDURE_EXIST_H

#include "components/source_subsystem/source_declarations.h"

namespace source {

class ProcedureExistException : public Exception {
 private:
  String m_msg;

 public:
  ProcedureExistException();
  [[nodiscard]] const char *what() const noexcept override;
};

}

#endif //PROCEDURE_EXIST_H
