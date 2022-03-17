#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_SOURCE_PARSER_SESSION_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_SOURCE_PARSER_SESSION_H_

#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "components/source_subsystem/exceptions/procedure_exist.h"

namespace source {

class SourceParserSession {
 private:
  std::string m_curr_parsed_procedure;
  std::unordered_set<std::string> m_procedures_parsed_set;
  std::unordered_set<std::string> m_procedures_called_set;
  std::unordered_map<std::string, std::unordered_set<std::string>> m_call_map; // <caller, callees>

 public:
  [[nodiscard]] bool DoesProcedureExist(std::string procedure_name);
  [[nodiscard]] bool DoesInvalidCallExist();
  [[nodiscard]] bool DoesCyclicCallExist();
  void AddProcedure(std::string procedure_name);
  void AddMethodCall(std::string callee_name);
  std::string GetCurrProcedure();
};

}

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_SOURCE_PARSER_SESSION_H_
