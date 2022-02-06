#include "parsed_query_validator.h"

#include <string>
#include <unordered_set>
#include <vector>

namespace pql_validator {

void ParsedQueryValidator::ValidateQuery(ParsedQuery query) {
  ValidateSelectSynonymDeclared(query);
  ValidateNoDuplicateSynonymDeclared(query);
}

void ParsedQueryValidator::ValidateSelectSynonymDeclared(ParsedQuery query) {
  std::vector<Declaration> declarations = query.GetDeclaration();
  std::string selected_synonym = query.GetSynonym().value;
  bool found = false;
  for (Declaration declaration : declarations) {
    if (declaration.GetSynonym().value==selected_synonym) {
      found = true;
    }
  }

  if (!found) {
    // TODO: throw exception
  }
}

void ParsedQueryValidator::ValidateNoDuplicateSynonymDeclared(ParsedQuery query) {
  std::vector<Declaration> declarations = query.GetDeclaration();
  std::unordered_set<std::string> synonym_set;
  for (Declaration declaration : declarations) {
    std::string synonym = declaration.GetSynonym().value;
    const bool is_in = synonym_set.find(synonym) != synonym_set.end();
    if (is_in) {
      // TODO: throw exception
    } else {
      synonym_set.insert(synonym);
    }
  }
}

}