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
  std::unordered_map<std::string, DesignEntityType> declarations = query.GetDeclaration();
  if (!query.GetRelationships().empty()) {
    for (auto relationship : query.GetRelationships()) {
      PqlTokenType rel_ref = relationship.GetRelRef().type;
      switch (rel_ref) {
        case PqlTokenType::FOLLOWS: {
          return ValidateFollowsFollowsTArguments(relationship, declarations);
        }
        case PqlTokenType::PARENT: {
          return ValidateParentParentTArguments(relationship, declarations);
        }
        case PqlTokenType::FOLLOWS_T: {
          return ValidateFollowsFollowsTArguments(relationship, declarations);
        }
        case PqlTokenType::PARENT_T: {
          return ValidateParentParentTArguments(relationship, declarations);
        }
        case PqlTokenType::USES: {
          return ValidateUsesArguments(relationship, declarations);
        }
        case PqlTokenType::MODIFIES: {
          return ValidateModifiesArguments(relationship, declarations);
        }
        case PqlTokenType::CALLS: {
          return ValidateCallsCallsTArguments(relationship, declarations);
        }
        case PqlTokenType::CALLS_T: {
          return ValidateCallsCallsTArguments(relationship, declarations);
        }
        case PqlTokenType::NEXT: {
          return ValidateNextNextTArguments(relationship, declarations);
        }
        case PqlTokenType::NEXT_T: {
          return ValidateNextNextTArguments(relationship, declarations);
        }
        case PqlTokenType::AFFECTS: {
          return ValidateAffectsAffectsTArguments(relationship, declarations);
        }
        case PqlTokenType::AFFECTS_T: {
          return ValidateAffectsAffectsTArguments(relationship, declarations);
        }
        default: {
          return false;
        }
      }
    }
  }

  return true;
}

bool ParsedQueryValidator::ValidateFollowsFollowsTArguments(Relationship relationship, std::unordered_map<std::string, DesignEntityType> declarations) {
  PqlToken first_arg = relationship.GetFirst();
  PqlToken second_arg = relationship.GetSecond();

  if (first_arg.type==PqlTokenType::SYNONYM && second_arg.type==PqlTokenType::SYNONYM) {
    if (first_arg.value==second_arg.value) {
      // Follows(s, s) is semantically invalid
      return false;
    }
  }
  return ValidateNextNextTArguments(relationship, declarations);
}

bool ParsedQueryValidator::ValidateParentParentTArguments(Relationship relationship, std::unordered_map<std::string, DesignEntityType> declarations) {
  // since Parent/Parent* parallels Follows/Follows* we use the same validation
  return ValidateFollowsFollowsTArguments(relationship, declarations);
}

bool ParsedQueryValidator::ValidateUsesArguments(Relationship relationship, std::unordered_map<std::string, DesignEntityType> declarations) {
  PqlToken first_arg = relationship.GetFirst();
  PqlToken second_arg = relationship.GetSecond();

  if (first_arg.type==PqlTokenType::SYNONYM && second_arg.type==PqlTokenType::SYNONYM) {
    if (declarations.find(first_arg.value) == declarations.end()) {
      return false;
    }
    if (declarations.find(second_arg.value) == declarations.end()) {
      return false;
    }

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

bool ParsedQueryValidator::ValidateModifiesArguments(Relationship relationship, std::unordered_map<std::string, DesignEntityType> declarations) {
  // same validation as Uses
  return ValidateUsesArguments(relationship, declarations);
}

bool ParsedQueryValidator::ValidateCallsCallsTArguments(Relationship relationship, std::unordered_map<std::string, DesignEntityType> declarations) {
  PqlToken first_arg = relationship.GetFirst();
  PqlToken second_arg = relationship.GetSecond();

  if (first_arg.type==PqlTokenType::SYNONYM && second_arg.type==PqlTokenType::SYNONYM) {
    if (declarations.find(first_arg.value) == declarations.end()) {
      return false;
    }
    if (declarations.find(second_arg.value) == declarations.end()) {
      return false;
    }

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
bool ParsedQueryValidator::ValidateNextNextTArguments(Relationship relationship, std::unordered_map<std::string, DesignEntityType> declarations) {
  PqlToken first_arg = relationship.GetFirst();
  PqlToken second_arg = relationship.GetSecond();

  if (first_arg.type==PqlTokenType::SYNONYM && second_arg.type==PqlTokenType::SYNONYM) {
    if (declarations.find(first_arg.value) == declarations.end()) {
      return false;
    }
    if (declarations.find(second_arg.value) == declarations.end()) {
      return false;
    }

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
bool ParsedQueryValidator::ValidateAffectsAffectsTArguments(Relationship relationship, std::unordered_map<std::string, DesignEntityType> declarations) {
  // same validation as Next.Next*
  return ValidateNextNextTArguments(relationship, declarations);
}

bool ParsedQueryValidator::ValidatePatternClause(ParsedQuery query) {
  std::vector<Pattern> patterns = query.GetPatterns();
  std::unordered_map<std::string, DesignEntityType> declarations = query.GetDeclaration();

  if (!patterns.empty()) {
    for (auto pattern : patterns) {
      PqlToken pattern_syn = pattern.GetSynonym();
      if (declarations.find(pattern_syn.value) == declarations.end()) {
        return false;
      }
      switch (pattern_syn.type) {
        case PqlTokenType::ASSIGN:
          return ValidateAssignPatternArguments(pattern, declarations);
        case PqlTokenType::WHILE:
          return ValidateWhilePatternArguments(pattern, declarations);
        case PqlTokenType::IF:
          return ValidateIfPatternArguments(pattern, declarations);
        default:
          return false;
      }
    }
  }

  return true;
}

bool ParsedQueryValidator::ValidateAssignPatternArguments(Pattern pattern, std::unordered_map<std::string, DesignEntityType> declarations) {
  PqlToken first_arg = pattern.GetFirst();
  PqlToken second_arg = pattern.GetSecond();

  if (first_arg.type==PqlTokenType::SYNONYM) {
    if (declarations.find(first_arg.value) == declarations.end()) {
      return false;
    }

    if (declarations.at(first_arg.value) != DesignEntityType::VARIABLE) {
      // pattern first argument synonym must be of type variable
      return false;
    }
  }
  return true;
}

bool ParsedQueryValidator::ValidateWhilePatternArguments(Pattern pattern, std::unordered_map<std::string, DesignEntityType> declarations) {
  PqlToken first_arg = pattern.GetFirst();
  PqlToken second_arg = pattern.GetSecond();

  if (first_arg.type==PqlTokenType::SYNONYM) {
    if (declarations.find(first_arg.value) == declarations.end()) {
      return false;
    }

    if (declarations.at(first_arg.value) != DesignEntityType::VARIABLE) {
      // pattern first argument synonym must be of type variable
      return false;
    }
  }

  if (second_arg.type != PqlTokenType::UNDERSCORE) {
    return false;
  }
  return true;
}

bool ParsedQueryValidator::ValidateIfPatternArguments(Pattern pattern, std::unordered_map<std::string, DesignEntityType> declarations) {
  PqlToken first_arg = pattern.GetFirst();
  PqlToken second_arg = pattern.GetSecond();
  PqlToken third_arg = pattern.GetThird();

  if (first_arg.type==PqlTokenType::SYNONYM) {
    if (declarations.find(first_arg.value) == declarations.end()) {
      return false;
    }

    if (declarations.at(first_arg.value) != DesignEntityType::VARIABLE) {
      // pattern first argument synonym must be of type variable
      return false;
    }
  }

  if (second_arg.type != PqlTokenType::UNDERSCORE || third_arg.type != PqlTokenType::UNDERSCORE) {
    return false;
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