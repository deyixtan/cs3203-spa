#include "parsed_query_validator.h"

#include <string>
#include <unordered_set>
#include <vector>

namespace pql_validator {

void ParsedQueryValidator::ValidateQuery(ParsedQuery query) {
  ValidateSelectSynonymDeclared(query);
  ValidateNoDuplicateSynonymDeclared(query);
  ValidatePatternSynonymIsAssigned(query);
  ValidateModifiesUsesFirstArgumentNotUnderscore(query);
  ValidateClauseDesignEntity(query);
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
  std::unordered_set<std::string> assign_synonym_set;
  std::string synonym = pattern.GetSynAssign().value;

  for (Declaration declaration : query.GetDeclaration()) {
    if (declaration.GetDesignEntity().type==PqlTokenType::ASSIGN) {
      assign_synonym_set.insert(declaration.GetSynonym().value);
    }
  }

  const bool is_in = assign_synonym_set.find(synonym)!=assign_synonym_set.end();

  if (!is_in) {
    // TODO: throw exception if pattern synonym is not declared
  }
}

void ParsedQueryValidator::ValidateModifiesUsesFirstArgumentNotUnderscore(ParsedQuery query) {
  std::vector<Relationship> relationships = query.GetRelationships();
  if (!relationships.empty()) {
    if (relationships.front().GetRelRef().type == PqlTokenType::MODIFIES ||
        relationships.front().GetRelRef().type == PqlTokenType::USES) {
      if (relationships.front().GetFirst().type == PqlTokenType::UNDERSCORE) {
        // TODO: throw exception if first argument of Modifies or Uses is _
      }
    }
  }
}

void ParsedQueryValidator::ValidateClauseDesignEntity(ParsedQuery query) {

  std::unordered_set<std::string> line_no_synonym_set;
  std::unordered_set<std::string> var_synonym_set;

  for (Declaration declaration : query.GetDeclaration()) {
    if (line_no_set.count(declaration.GetDesignEntity().type)) {
      line_no_synonym_set.insert(declaration.GetSynonym().value);
    } else if (var_set.count(declaration.GetDesignEntity().type)) {
      var_synonym_set.insert(declaration.GetSynonym().value);
    }
  }

  std::vector<Relationship> relationships = query.GetRelationships();
  if (!relationships.empty()) {
    if (relationships.front().GetRelRef().type == PqlTokenType::MODIFIES ||
        relationships.front().GetRelRef().type == PqlTokenType::USES) {
      if (!line_no_synonym_set.count(relationships.front().GetFirst().value)) {
        // TODO: throw exception if first argument of Modifies/Uses is not a line no synonym
      }
      if (!var_synonym_set.count(relationships.front().GetSecond().value)) {
        // TODO: throw exception if second argument of Modifies/Uses is not a var synonym
      }
    } else if (relationships.front().GetRelRef().type == PqlTokenType::FOLLOWS_T ||
               relationships.front().GetRelRef().type == PqlTokenType::FOLLOWS ||
               relationships.front().GetRelRef().type == PqlTokenType::PARENT_T ||
               relationships.front().GetRelRef().type == PqlTokenType::PARENT) {
      if (!line_no_synonym_set.count(relationships.front().GetFirst().value)) {
        // TODO: throw exception if first argument of Follows/Parent is not a line no synonym
      }
      if (!line_no_synonym_set.count(relationships.front().GetSecond().value)) {
        // TODO: throw exception if second argument of Follows/Parent is not a line no synonym
      }
    }
  }
}

}