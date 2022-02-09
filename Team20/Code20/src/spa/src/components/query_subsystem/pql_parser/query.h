#ifndef SPA_SRC_SPA_SRC_COMPONENTS_QUERY_SUBSYSTEM_PQL_PARSER_QUERY_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_QUERY_SUBSYSTEM_PQL_PARSER_QUERY_H_

#include <string>
#include <utility>
#include <unordered_map>
#include <vector>

namespace pql {

using std::pair;
using std::string;
using std::unordered_map;
using std::vector;

class Query {
 public:
  void AddDeclaration(string ident, string design_entity);
  void SetSelectedSynonym(string ident);
  void AddSuchThatClause(pair<string, pair<string, string>> such_that_clause);
  void AddPatternClause(pair<string, pair<string, string>> pattern_clause);
  string GetSelectedSynonym() const;
  vector<pair<string, pair<string, string>>> GetSuchThatClauses() const;
  vector<pair<string, pair<string, string>>> GetPatternClauses() const;

 private:
  unordered_map<string, string> declarations; // IDENT -> design-entity
  string selected_syonym; // IDENT
  vector<pair<string, pair<string, string>>> such_that_clauses; // [ (relRef, (arg1, arg2)) ]
  vector<pair<string, pair<string, string>>> pattern_clauses; // [ (pattern-syn, (arg1, arg2)) ]
};

}

#endif //SPA_SRC_SPA_SRC_COMPONENTS_QUERY_SUBSYSTEM_PQL_PARSER_QUERY_H_
