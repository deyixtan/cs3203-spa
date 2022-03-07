#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_SOURCE_PARSER_SESSION_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_SOURCE_PARSER_SESSION_H_

#include "string"
#include "unordered_set"

#include "exceptions/procedure_exist.h"

namespace source {

class SourceParserSession {
 private:
  std::unordered_set<std::string> m_procedure_name_set;

 private:
  [[nodiscard]] bool DoesProcedureExist(std::string procedure_name);

 public:
  void AddProcedure(std::string procedure_name);
};

}

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_SOURCE_PARSER_SESSION_H_
