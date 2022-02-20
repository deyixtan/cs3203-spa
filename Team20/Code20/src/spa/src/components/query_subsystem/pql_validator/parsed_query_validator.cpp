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
    Relationship relationship = query.GetRelationships().front();
    PqlTokenType rel_ref = relationship.GetRelRef().type;
    switch (rel_ref) {
      case PqlTokenType::FOLLOWS: {
        return ValidateFollowsFollowsTArguments(query);
      }
      case PqlTokenType::PARENT: {
        return ValidateParentParentTArguments(query);
      }
      case PqlTokenType::FOLLOWS_T: {
        return ValidateFollowsFollowsTArguments(query);
      }
      case PqlTokenType::PARENT_T: {
        return ValidateParentParentTArguments(query);
      }
      case PqlTokenType::USES: {
        return ValidateUsesArguments(query);
      }
      case PqlTokenType::MODIFIES: {
        return ValidateModifiesArguments(query);
      }
      default: {
        return false;
      }
    }
  }

  return true;
}

bool ParsedQueryValidator::ValidateFollowsFollowsTArguments(ParsedQuery query) {
  assert(!query.GetRelationships().empty());
  Relationship relationship = query.GetRelationships().front();
  PqlToken first_arg = relationship.GetFirst();
  PqlToken second_arg = relationship.GetSecond();

  if (!IsStmtRef(first_arg.type)) {
    return false;
  }
  if (!IsStmtRef(second_arg.type)) {
    return false;
  }

  if (first_arg.type==PqlTokenType::SYNONYM && second_arg.type==PqlTokenType::SYNONYM) {
    if (first_arg.value==second_arg.value) {
      // Follows(s, s) is semantically invalid
      return false;
    }

    bool found_first = false;
    PqlTokenType first_arg_design_entity;
    bool found_second = false;
    PqlTokenType second_arg_design_entity;
    for (auto declaration : query.GetDeclaration()) {
      if (declaration.GetSynonym().value==first_arg.value) {
        found_first = true;
        first_arg_design_entity = declaration.GetDesignEntity().type;
      }
      if (declaration.GetSynonym().value==second_arg.value) {
        found_second = true;
        second_arg_design_entity = declaration.GetDesignEntity().type;
      }
    }

    if (!found_first || !found_second) {
      return false;
    }
    if (!IsStmt(first_arg_design_entity)) {
      return false;
    }
    if (!IsStmt(second_arg_design_entity)) {
      return false;
    }
  } else if (first_arg.type==PqlTokenType::SYNONYM) {
    bool found_first = false;
    PqlTokenType first_arg_design_entity;
    for (auto declaration : query.GetDeclaration()) {
      if (declaration.GetSynonym().value==first_arg.value) {
        found_first = true;
        first_arg_design_entity = declaration.GetDesignEntity().type;
      }
    }

    if (!found_first) {
      return false;
    }
    if (!IsStmt(first_arg_design_entity)) {
      return false;
    }
  } else if (second_arg.type==PqlTokenType::SYNONYM) {
    bool found_second = false;
    PqlTokenType second_arg_design_entity;
    for (auto declaration : query.GetDeclaration()) {
      if (declaration.GetSynonym().value==second_arg.value) {
        found_second = true;
        second_arg_design_entity = declaration.GetDesignEntity().type;
      }
    }

    if (!found_second) {
      return false;
    }
    if (!IsStmt(second_arg_design_entity)) {
      return false;
    }
  }

  return true;
}

bool ParsedQueryValidator::ValidateParentParentTArguments(ParsedQuery query) {
  // since Parent/Parent* parallels Follows/Follows* we use the same validation
  return ValidateFollowsFollowsTArguments(query);
}

bool ParsedQueryValidator::ValidateUsesArguments(ParsedQuery query) {
  assert(!query.GetRelationships().empty());
  Relationship relationship = query.GetRelationships().front();
  PqlToken first_arg = relationship.GetFirst();
  PqlToken second_arg = relationship.GetSecond();

  if (first_arg.type!=PqlTokenType::SYNONYM
      && first_arg.type!=PqlTokenType::NUMBER) {
    return false;
  }
  if (!IsEntRef(second_arg.type)) {
    return false;
  }

  if (first_arg.type==PqlTokenType::SYNONYM && second_arg.type==PqlTokenType::SYNONYM) {
    if (first_arg.value==second_arg.value) {
      // Uses(s, s) is semantically invalid
      return false;
    }

    bool found_first = false;
    PqlTokenType first_arg_design_entity;
    bool found_second = false;
    PqlTokenType second_arg_design_entity;
    for (auto declaration : query.GetDeclaration()) {
      if (declaration.GetSynonym().value==first_arg.value) {
        found_first = true;
        first_arg_design_entity = declaration.GetDesignEntity().type;
      }
      if (declaration.GetSynonym().value==second_arg.value) {
        found_second = true;
        second_arg_design_entity = declaration.GetDesignEntity().type;
      }
    }

    if (!found_first || !found_second) {
      return false;
    }
    if (!IsStmt(first_arg_design_entity)) {
      return false;
    }
    if (second_arg_design_entity!=PqlTokenType::VARIABLE) {
      return false;
    }
  } else if (first_arg.type==PqlTokenType::SYNONYM) {
    bool found_first = false;
    PqlTokenType first_arg_design_entity;
    for (auto declaration : query.GetDeclaration()) {
      if (declaration.GetSynonym().value==first_arg.value) {
        found_first = true;
        first_arg_design_entity = declaration.GetDesignEntity().type;
      }
    }

    if (!found_first) {
      return false;
    }
    if (!IsStmt(first_arg_design_entity)) {
      return false;
    }
  } else if (second_arg.type==PqlTokenType::SYNONYM) {
    bool found_second = false;
    PqlTokenType second_arg_design_entity;
    for (auto declaration : query.GetDeclaration()) {
      if (declaration.GetSynonym().value==second_arg.value) {
        found_second = true;
        second_arg_design_entity = declaration.GetDesignEntity().type;
      }
    }

    if (!found_second) {
      return false;
    }
    if (second_arg_design_entity!=PqlTokenType::VARIABLE) {
      return false;
    }
  }

  return true;
}

bool ParsedQueryValidator::ValidateModifiesArguments(ParsedQuery query) {
  // same validation as Uses
  return ValidateUsesArguments(query);
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
  PqlToken first_arg = query.GetPatterns().front().GetFirst();
  PqlToken second_arg = query.GetPatterns().front().GetSecond();

  if (!IsEntRef(first_arg.type)) {
    return false;
  }
  if (!IsExpressionSpec(second_arg.type)) {
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

    if (first_arg_design_entity!=PqlTokenType::VARIABLE) {
      // pattern first argument synonym must be of type variable
      return false;
    }
  }

  return true;
}

bool ParsedQueryValidator::IsStmt(PqlTokenType token_type) {
  return token_type==PqlTokenType::STMT
      || token_type==PqlTokenType::READ
      || token_type==PqlTokenType::PRINT
      || token_type==PqlTokenType::CALL
      || token_type==PqlTokenType::WHILE
      || token_type==PqlTokenType::IF
      || token_type==PqlTokenType::ASSIGN;
}

bool ParsedQueryValidator::IsStmtRef(PqlTokenType token_type) {
  return token_type==PqlTokenType::SYNONYM
      || token_type==PqlTokenType::UNDERSCORE
      || token_type==PqlTokenType::NUMBER;
}

bool ParsedQueryValidator::IsEntRef(PqlTokenType token_type) {
  return token_type==PqlTokenType::SYNONYM
      || token_type==PqlTokenType::UNDERSCORE
      || token_type==PqlTokenType::IDENT_WITH_QUOTES;
}

bool ParsedQueryValidator::IsExpressionSpec(PqlTokenType token_type) {
  // exact match not in iter 1
  return token_type==PqlTokenType::UNDERSCORE
      || token_type==PqlTokenType::SUB_EXPRESSION;
}

}