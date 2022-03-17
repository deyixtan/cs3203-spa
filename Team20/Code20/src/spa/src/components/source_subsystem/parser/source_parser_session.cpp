#include "source_parser_session.h"

namespace source {

bool SourceParserSession::DoesProcedureExist(std::string procedure_name) {
  return m_procedures_parsed_set.find(procedure_name) != m_procedures_parsed_set.end();
}

bool SourceParserSession::DoesInvalidCallExist() {
  for (auto const &procedure_called : m_procedures_called_set) {
    if (m_procedures_parsed_set.find(procedure_called) == m_procedures_parsed_set.end()) {
      return true;
    }
  }
  return false;
}

bool SourceParserSession::DoesCyclicCallExist() {
  // using Kahn algorithm to check for cycles
  // NOTE1: may need to optimise/abstract to another class.
  // NOTE2: perhaps should convert to Tarjan's algorithm for better efficiency

  // init in-degree map
  std::unordered_map<std::string, int> in_degree_map;
  for (auto const &procedure_parsed : m_procedures_parsed_set) {
    in_degree_map.insert({procedure_parsed, 0});
  }

  // update in-degree of each procedure
  for (auto const &procedure_parsed : m_procedures_parsed_set) {
    std::unordered_set callees = m_call_map.at(procedure_parsed);
    for (auto const &callee : callees) {
      in_degree_map.at(callee) += 1;
    }
  }

  // populate queue with procedure with 0 in-degree
  std::queue<std::string> queue;
  for (auto const &[caller_name, in_degree_count] : in_degree_map) {
    if (in_degree_count == 0) {
      queue.push(caller_name);
    }
  }

  // process each procedure with 0 in-degree
  // at the end, if managed to process the same number of procedures being parsed = no cycles
  int processed = 0;
  while (!queue.empty()) {
    std::string curr = queue.front();
    queue.pop();

    std::unordered_set callees = m_call_map.at(curr);
    for (auto const &callee : callees) {
      in_degree_map.at(callee) -= 1;
      if (in_degree_map.at(callee) == 0) {
        queue.push(callee);
      }
    }

    processed += 1;
  }

  return processed != m_procedures_parsed_set.size();
}

void SourceParserSession::AddProcedure(std::string procedure_name) {
  if (DoesProcedureExist(procedure_name)) {
    throw ProcedureExistException();
  }

  m_curr_parsed_procedure = procedure_name;
  m_procedures_parsed_set.insert(m_curr_parsed_procedure);

  m_call_map.insert({m_curr_parsed_procedure, std::unordered_set<std::string>()});
}

void SourceParserSession::AddMethodCall(std::string callee_name) {
  m_procedures_called_set.insert(callee_name);
  m_call_map.at(m_curr_parsed_procedure).insert(callee_name);
}

}
