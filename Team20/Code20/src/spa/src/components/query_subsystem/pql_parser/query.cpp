#include "query.h"

namespace pql {

void Query::AddDeclaration(string ident, string design_entity) {
  declarations.insert(ident, design_entity);
}

void Query::SetSelectedSynonym(string ident) {
  selected_synonym = ident;
}

void Query::AddSuchThatClause(string rel_ref, string arg1, string arg2) {
  pair<string, string> args {arg1, arg2};
  pair<string, pair<string, string>> such_that_clause {rel_ref, args};
  such_that_clauses.push_back(such_that_clause);
}

void Query::AddPatternClause(string pattern_syn, string arg1, string arg2) {
  pair<string, string> args {arg1, arg2};
  pair<string, pair<string, string>> pattern_clause {pattern_syn, args};
  pattern_clauses.push_back(pattern_clause);
}

string Query::GetSelectedSynonym() const {
  return selected_synonym;
}

vector<pair<string, pair<string, string>>> Query::GetSuchThatClauses() const {
  return such_that_clauses;
}

vector<pair<string, pair<string, string>>> Query::GetPatternClauses() const {
  return pattern_clauses;
}

}