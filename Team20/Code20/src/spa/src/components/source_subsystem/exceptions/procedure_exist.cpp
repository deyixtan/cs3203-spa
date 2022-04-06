#include "procedure_exist.h"

namespace source {

ProcedureExistException::ProcedureExistException() : m_msg("Duplicate procedure name exists.") {}

const char *ProcedureExistException::what() const noexcept {
  return m_msg.c_str();
}

}
