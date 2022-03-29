#include "procedure_exist.h"

namespace source {

std::string ProcedureExistException::MESSAGE = "Duplicate procedure name Valid.";

ProcedureExistException::ProcedureExistException() : m_msg(MESSAGE) {}

const char *ProcedureExistException::what() const noexcept {
  return m_msg.c_str();
}

}
