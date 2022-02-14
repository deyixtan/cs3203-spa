#include "parsed_query_validator.h"

#include <string>
#include <unordered_set>
#include <vector>

namespace pql_validator {

void ParsedQueryValidator::ValidateQuery(ParsedQuery query) {
  ValidateSelectSynonymDeclared(query);
  ValidateNoDuplicateSynonymDeclared(query);
  ValidatePatternSynonymIsAssigned(query);
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
    const bool is_in = synonym_set.find(synonym)!=synonym_set.end();
    if (is_in) {
      // TODO: throw exception
    } else {
      synonym_set.insert(synonym);
    }
  }
}

void ParsedQueryValidator::ValidatePatternSynonymIsAssigned(ParsedQuery query) {
  // assume only single pattern for now
  if (query.GetPatterns().empty()) {
    return;
  }

  Pattern pattern = query.GetPatterns().front();
  std::unordered_set<std::string> assign_syonym_set;
  std::string synonym = pattern.GetSynAssign().value;

  for (Declaration declaration : query.GetDeclaration()) {
    if (declaration.GetDesignEntity().type==PqlTokenType::ASSIGN) {
      assign_syonym_set.insert(declaration.GetSynonym().value);
    }
  }

  const bool is_in = assign_syonym_set.find(synonym)!=assign_syonym_set.end();

  if (!is_in) {
    // TODO: throw exception if pattern synonym is not declared
  }
}

}