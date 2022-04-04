#ifndef SOURCE_PARSER_SESSION_H
#define SOURCE_PARSER_SESSION_H

#include "components/source_subsystem/source_declarations.h"

namespace source {

class SourceParserSession {
 private:
  String m_curr_parsed_procedure;
  StringSet m_procedures_parsed_set;
  StringSet m_procedures_called_set;
  StringToStringSetMap m_call_map; // <caller, callees>

 private:
  [[nodiscard]] StringToIntMap GetInitKahnInDegreeMap();
  [[nodisacrd]] StringQueue GetInitKahnQueue(StringToIntMap &in_degree_map);
  void ProcessKahnQueue(StringToIntMap &in_degree_map, StringQueue &queue, int &processed);

 public:
  [[nodiscard]] String GetCurrProcedure();
  [[nodiscard]] bool DoesProcedureExist(String const &procedure_name);
  [[nodiscard]] bool DoesInvalidCallExist();
  [[nodiscard]] bool DoesCyclicCallExist();
  void AddProcedure(String const &procedure_name);
  void AddMethodCall(String const &callee_name);
};

}

#endif //SOURCE_PARSER_SESSION_H
