#include "parsed_query_validator.h"

#include <string>
#include <unordered_set>
#include <vector>

namespace pql_validator {

bool ParsedQueryValidator::ValidateQuery(ParsedQuery query) {
  return ValidateResultClauseDeclared(query)
      && ValidateSuchThatClause(query)
      && ValidatePatternClause(query);
}

bool ParsedQueryValidator::ValidateAttribute(PqlToken token, std::unordered_map<std::string, DesignEntityType> declarations) {
  static std::pair<std::pair<DesignEntityType, std::string>, AtrriName> parsed_attribute = Utils::ParseAttributeRef(token, declarations);
  if (parsed_attribute.first.first == DesignEntityType::PROCEDURE) {
    if (parsed_attribute.second != AtrriName::PROCNAME) {
      return false;
    }
  } else if (parsed_attribute.first.first == DesignEntityType::STMT ||
      parsed_attribute.first.first == DesignEntityType::WHILE ||
      parsed_attribute.first.first == DesignEntityType::IF ||
      parsed_attribute.first.first == DesignEntityType::ASSIGN) {
    if (parsed_attribute.second != AtrriName::STMTNO) {
      return false;
    }
  } else if (parsed_attribute.first.first == DesignEntityType::CONSTANT) {
    if (parsed_attribute.second != AtrriName::VALUE) {
      return false;
    }
  } else if (parsed_attribute.first.first == DesignEntityType::READ ||
      parsed_attribute.first.first == DesignEntityType::PRINT) {
    if (parsed_attribute.second != AtrriName::VARNAME &&
        parsed_attribute.second != AtrriName::STMTNO) {
      return false;
    }
  } else if (parsed_attribute.first.first == DesignEntityType::CALL) {
    if (parsed_attribute.second != AtrriName::PROCNAME &&
        parsed_attribute.second != AtrriName::STMTNO) {
      return false;
    }
  }
}

bool ParsedQueryValidator::ValidateResultClauseDeclared(ParsedQuery query) {
  std::unordered_map<std::string, DesignEntityType> declarations = query.GetDeclaration();
  ResultClause result_clause = query.GetResultClause();

  for (auto token : result_clause.GetValues()) {
    if (token.type == PqlTokenType::SYNONYM) {
      if (declarations.find(token.value) == declarations.end()) {
        return false;
      }
    } else if (token.type == PqlTokenType::ATTRIBUTE) {
      std::pair<std::string, AtrriName> parsedAttribute = Utils::ParseAttributeRef(token);
      if (declarations.find(parsedAttribute.first) == declarations.end()) {
        return false;
      }
      return ValidateAttribute(token, declarations);
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
      case PqlTokenType::CALLS: {
        return ValidateCallsCallsTArguments(query);
      }
      case PqlTokenType::CALLS_T: {
        return ValidateCallsCallsTArguments(query);
      }
      case PqlTokenType::NEXT: {
        return ValidateNextNextTArguments(query);
      }
      case PqlTokenType::NEXT_T: {
        return ValidateNextNextTArguments(query);
      }
      case PqlTokenType::AFFECTS: {
        return ValidateAffectsAffectsTArguments(query);
      }
      case PqlTokenType::AFFECTS_T: {
        return ValidateAffectsAffectsTArguments(query);
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
  std::unordered_map<std::string, DesignEntityType> declarations = query.GetDeclaration();

  if (first_arg.type==PqlTokenType::SYNONYM && second_arg.type==PqlTokenType::SYNONYM) {
    if (first_arg.value==second_arg.value) {
      // Follows(s, s) is semantically invalid
      return false;
    }
  }
  return ValidateNextNextTArguments(query);
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
  std::unordered_map<std::string, DesignEntityType> declarations = query.GetDeclaration();

  if (first_arg.type==PqlTokenType::SYNONYM && second_arg.type==PqlTokenType::SYNONYM) {
    if (first_arg.value==second_arg.value) {
      // Uses(s, s) is semantically invalid
      return false;
    }

    if (!IsStmt(declarations.at(first_arg.value)) && declarations.at(first_arg.value) != DesignEntityType::PROCEDURE) {
      return false;
    }
    if (declarations.at(second_arg.value) !=DesignEntityType::VARIABLE) {
      return false;
    }
  } else if (first_arg.type == PqlTokenType::SYNONYM) {
    if (!IsStmt(declarations.at(first_arg.value))) {
      return false;
    }
  } else if (second_arg.type == PqlTokenType::SYNONYM) {
    if (declarations.at(second_arg.value)!= DesignEntityType::VARIABLE) {
      return false;
    }
  }

  return true;
}

bool ParsedQueryValidator::ValidateModifiesArguments(ParsedQuery query) {
  // same validation as Uses
  return ValidateUsesArguments(query);
}

bool ParsedQueryValidator::ValidateCallsCallsTArguments(ParsedQuery query) {
  assert(!query.GetRelationships().empty());
  Relationship relationship = query.GetRelationships().front();
  PqlToken first_arg = relationship.GetFirst();
  PqlToken second_arg = relationship.GetSecond();
  std::unordered_map<std::string, DesignEntityType> declarations = query.GetDeclaration();

  if (first_arg.type==PqlTokenType::SYNONYM && second_arg.type==PqlTokenType::SYNONYM) {
    if (first_arg.value==second_arg.value) {
      // Calls(p, p) is semantically invalid due to cyclic call
      return false;
    }
    if (!IsProc(declarations.at(first_arg.value))) {
      return false;
    }
    if (!IsProc(declarations.at(second_arg.value))) {
      return false;
    }
  } else if (first_arg.type==PqlTokenType::SYNONYM) {
    if (!IsProc(declarations.at(first_arg.value))) {
      return false;
    }
  } else if (second_arg.type==PqlTokenType::SYNONYM) {
    if (!IsProc(declarations.at(second_arg.value))) {
      return false;
    }
  }
  return true;
}
bool ParsedQueryValidator::ValidateNextNextTArguments(ParsedQuery query) {
  assert(!query.GetRelationships().empty());
  Relationship relationship = query.GetRelationships().front();
  PqlToken first_arg = relationship.GetFirst();
  PqlToken second_arg = relationship.GetSecond();
  std::unordered_map<std::string, DesignEntityType> declarations = query.GetDeclaration();

  if (first_arg.type==PqlTokenType::SYNONYM && second_arg.type==PqlTokenType::SYNONYM) {
    if (!IsStmt(declarations.at(first_arg.value))) {
      return false;
    }
    if (!IsStmt(declarations.at(second_arg.value))) {
      return false;
    }
  } else if (first_arg.type==PqlTokenType::SYNONYM) {
    if (!IsStmt(declarations.at(first_arg.value))) {
      return false;
    }
  } else if (second_arg.type==PqlTokenType::SYNONYM) {
    if (!IsStmt(declarations.at(second_arg.value))) {
      return false;
    }
  }
  return true;
}
bool ParsedQueryValidator::ValidateAffectsAffectsTArguments(ParsedQuery query) {
  // same validation as Next.Next*
  return ValidateNextNextTArguments(query);
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

  for (auto declaration : query.GetDeclaration()) {
    if (declaration.second == DesignEntityType::ASSIGN) {
      assign_synonyms_declared.insert(declaration.first);
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
    DesignEntityType first_arg_design_entity;
    for (auto declaration : query.GetDeclaration()) {
      if (declaration.first == first_arg.value) {
        found = true;
        first_arg_design_entity = declaration.second;
      }
    }

    if (!found) {
      // synonym not declared
      return false;
    }

    if (first_arg_design_entity != DesignEntityType::VARIABLE) {
      // pattern first argument synonym must be of type variable
      return false;
    }
  }

  return true;
}

bool ParsedQueryValidator::IsProc(DesignEntityType token_type) {
  return token_type==DesignEntityType::PROCEDURE;
}

bool ParsedQueryValidator::IsStmt(DesignEntityType token_type) {
  return token_type==DesignEntityType::STMT
      || token_type==DesignEntityType::READ
      || token_type==DesignEntityType::PRINT
      || token_type==DesignEntityType::CALL
      || token_type==DesignEntityType::WHILE
      || token_type==DesignEntityType::IF
      || token_type==DesignEntityType::ASSIGN;
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