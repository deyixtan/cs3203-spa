#include "parsed_query_validator.h"

#include <string>
#include <unordered_set>
#include <vector>

namespace pql_validator {

bool ParsedQueryValidator::ValidateQuery(ParsedQuery query) {
  return ValidateSelectSynonymDeclared(query)
      && ValidateNoDuplicateSynonymDeclared(query)
      && ValidateSuchThatClause(query)
      && ValidatePatternClause(query);

//      && ValidatePatternSynonymIsAssigned(query)
//      && ValidateModifiesUsesFirstArgumentNotUnderscore(query)
//      && ValidateClauseDesignEntity(query);
}

bool ParsedQueryValidator::ValidateSelectSynonymDeclared(ParsedQuery query) {
  std::vector<Declaration> declarations = query.GetDeclaration();
  std::string selected_synonym = query.GetSynonym().value;
  bool found = false;
  for (Declaration declaration : declarations) {
    if (declaration.GetSynonym().value==selected_synonym) {
      found = true;
    }
  }

  return found;
}

bool ParsedQueryValidator::ValidateNoDuplicateSynonymDeclared(ParsedQuery query) {
  std::vector<Declaration> declarations = query.GetDeclaration();
  std::unordered_set<std::string> synonym_set;
  for (Declaration declaration : declarations) {
    std::string synonym = declaration.GetSynonym().value;
    const bool is_in = synonym_set.find(synonym)!=synonym_set.end();
    if (is_in) {
      // has duplicate
      return false;
    } else {
      synonym_set.insert(synonym);
    }
  }

  return true;
}

bool ParsedQueryValidator::ValidateSuchThatClause(ParsedQuery query) {
  if (!query.GetRelationships().empty()) {

  }

  return true;
}

bool ParsedQueryValidator::ValidatePatternClause(ParsedQuery query) {
  if (!query.GetPatterns().empty()) {
    return ValidatePatternSynonymIsAssigned(query)
        && ValidatePatternArguments(query);
  }

  return true;
}

bool ParsedQueryValidator::ValidatePatternSynonymIsAssigned(ParsedQuery query) {
  assert(!query.GetPatterns().empty());
  Pattern pattern = query.GetPatterns().front();
  std::string pattern_synonym = pattern.GetSynAssign().value; // only assign for now
  std::unordered_set<std::string> assign_synonyms_declared;

  for (Declaration declaration : query.GetDeclaration()) {
    if (declaration.GetDesignEntity().type==PqlTokenType::ASSIGN) {
      assign_synonyms_declared.insert(declaration.GetSynonym().value);
    }
  }

  const bool is_in = assign_synonyms_declared.find(pattern_synonym)!=assign_synonyms_declared.end();

  return is_in;
}

bool ParsedQueryValidator::ValidatePatternArguments(ParsedQuery query) {
  assert(!query.GetPatterns().empty());
  return ValidatePatternFirstArgumentIsEntRef(query)
      && ValidatePatternSecondArgumentIsExpressionSpec(query);
}

bool ParsedQueryValidator::ValidatePatternFirstArgumentIsEntRef(ParsedQuery query) {
  assert(!query.GetPatterns().empty());
  PqlToken first_arg = query.GetPatterns().front().GetFirst();
  if (first_arg.type!=PqlTokenType::SYNONYM
      && first_arg.type!=PqlTokenType::UNDERSCORE
      && first_arg.type!=PqlTokenType::IDENT_WITH_QUOTES) {
    return false;
  }

  if (first_arg.type==PqlTokenType::SYNONYM) {
    bool found = false;
    PqlTokenType first_arg_design_entity;
    for (auto declaration : query.GetDeclaration()) {
      if (declaration.GetSynonym().value==first_arg.value) {
        found = true;
        first_arg_design_entity = declaration.GetDesignEntity().type;
      }
    }

    if (!found) {
      // synonym not declared
      return false;
    }
  }

  return true;
}

bool ParsedQueryValidator::ValidatePatternSecondArgumentIsExpressionSpec(ParsedQuery query) {
  assert(!query.GetPatterns().empty());
  PqlToken second_arg = query.GetPatterns().front().GetSecond();
  if (second_arg.type!=PqlTokenType::UNDERSCORE
      && second_arg.type!=PqlTokenType::SUB_EXPRESSION) {
    return false;
  }

  return true;
}

bool ParsedQueryValidator::ValidateModifiesUsesFirstArgumentNotUnderscore(ParsedQuery query) {
  std::vector<Relationship> relationships = query.GetRelationships();
  if (!relationships.empty()) {
    if (relationships.front().GetRelRef().type==PqlTokenType::MODIFIES ||
        relationships.front().GetRelRef().type==PqlTokenType::USES) {
      if (relationships.front().GetFirst().type==PqlTokenType::UNDERSCORE) {
        return false;
      }
    }
  }
  return true;
}

bool ParsedQueryValidator::ValidateClauseDesignEntity(ParsedQuery query) {

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
    if (relationships.front().GetRelRef().type==PqlTokenType::MODIFIES ||
        relationships.front().GetRelRef().type==PqlTokenType::USES) {
      if (!line_no_synonym_set.count(relationships.front().GetFirst().value)) {
        // TODO: throw exception if first argument of Modifies/Uses is not a line no synonym
      }
      if (!var_synonym_set.count(relationships.front().GetSecond().value)) {
        // TODO: throw exception if second argument of Modifies/Uses is not a var synonym
      }
    } else if (relationships.front().GetRelRef().type==PqlTokenType::FOLLOWS_T ||
        relationships.front().GetRelRef().type==PqlTokenType::FOLLOWS ||
        relationships.front().GetRelRef().type==PqlTokenType::PARENT_T ||
        relationships.front().GetRelRef().type==PqlTokenType::PARENT) {
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