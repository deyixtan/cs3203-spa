#include "source_parser_session.h"

namespace source {

bool SourceParserSession::DoesProcedureExist(std::string procedure_name) {
  return m_procedure_name_set.find(procedure_name) != m_procedure_name_set.end();
}

void SourceParserSession::AddProcedure(std::string procedure_name) {
  if (DoesProcedureExist(procedure_name)) {
    throw ProcedureExistException();
  }

  m_procedure_name_set.insert(procedure_name);
}

}
