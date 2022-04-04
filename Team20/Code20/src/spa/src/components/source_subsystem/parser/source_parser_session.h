#ifndef SOURCE_PARSER_SESSION_H
#define SOURCE_PARSER_SESSION_H

#include "components/source_subsystem/source_declarations.h"

namespace source {

class SourceParserSession {
 private:
  std::string m_curr_parsed_procedure;
  std::unordered_set<std::string> m_procedures_parsed_set;
  std::unordered_set<std::string> m_procedures_called_set;
  std::unordered_map<std::string, std::unordered_set<std::string>> m_call_map; // <caller, callees>

 private:
  [[nodiscard]] std::unordered_map<std::string, int> GetInitKahnInDegreeMap();
  [[nodisacrd]] std::queue<std::string> GetInitKahnQueue(std::unordered_map<std::string, int> &in_degree_map);
  void ProcessKahnQueue(std::unordered_map<std::string, int> &in_degree_map, std::queue<std::string> &queue, int &processed);

 public:
  [[nodiscard]] std::string GetCurrProcedure();
  [[nodiscard]] bool DoesProcedureExist(std::string const &procedure_name);
  [[nodiscard]] bool DoesInvalidCallExist();
  [[nodiscard]] bool DoesCyclicCallExist();
  void AddProcedure(std::string const &procedure_name);
  void AddMethodCall(std::string const &callee_name);
};

}

#endif //SOURCE_PARSER_SESSION_H
