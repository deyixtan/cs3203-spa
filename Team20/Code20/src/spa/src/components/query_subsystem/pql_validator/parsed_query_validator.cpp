#include "parsed_query_validator.h"

#include <string>
#include <vector>

namespace pql_validator {

bool ParsedQueryValidator::ValidateQuery(ParsedQuery query) {
  return ValidateResultClauseDeclared(query)
      && ValidateSuchThatClause(query)
      && ValidatePatternClause(query)
      && ValidateWithClause(query);
}

bool ParsedQueryValidator::ValidateAttribute(PqlToken token, std::unordered_map<std::string, DesignEntityType> declarations) {
  std::pair<std::string, AtrriName> parsed_attribute_raw = Utils::ParseAttributeRef(token);

  if (declarations.count(parsed_attribute_raw.first) == 0) {
    return false;
  }
  std::pair<std::pair<DesignEntityType, std::string>, AtrriName> parsed_attribute = Utils::ParseAttributeRef(token, declarations);

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
  } else if (parsed_attribute.first.first == DesignEntityType::VARIABLE) {
    if (parsed_attribute.second != AtrriName::VARNAME) {
      return false;
    }
  }
  return true;
}

bool ParsedQueryValidator::ValidateResultClauseDeclared(ParsedQuery query) {
  std::unordered_map<std::string, DesignEntityType> declarations = query.GetDeclaration();
  ResultClause result_clause = query.GetResultClause();
  bool is_valid = true;
  for (auto token : result_clause.GetValues()) {
    if (token.type == PqlTokenType::SYNONYM) {
      if (declarations.count(token.value) == 0) {
        return false;
      }
    } else if (token.type == PqlTokenType::ATTRIBUTE) {
      std::pair<std::string, AtrriName> parsedAttribute = Utils::ParseAttributeRef(token);
      if (declarations.count(parsedAttribute.first) == 0) {
        return false;
      }
      is_valid = is_valid && ValidateAttribute(token, declarations);
    }
  }
  return is_valid;
}

bool ParsedQueryValidator::ValidateSuchThatClause(ParsedQuery query) {
  std::unordered_map<std::string, DesignEntityType> declarations = query.GetDeclaration();
  bool is_valid = true;
  if (!query.GetRelationships().empty()) {
    for (auto relationship : query.GetRelationships()) {
      PqlTokenType rel_ref = relationship.GetRelRef().type;
      if (rel_ref == PqlTokenType::FOLLOWS) {
        is_valid = is_valid && ValidateFollowsFollowsTArguments(relationship, declarations);
      } else if (rel_ref == PqlTokenType::PARENT) {
        is_valid = is_valid && ValidateParentParentTArguments(relationship, declarations);
      } else if (rel_ref == PqlTokenType::FOLLOWS_T) {
        is_valid = is_valid && ValidateFollowsFollowsTArguments(relationship, declarations);
      } else if (rel_ref == PqlTokenType::PARENT_T) {
        is_valid = is_valid && ValidateParentParentTArguments(relationship, declarations);
      } else if (rel_ref == PqlTokenType::USES) {
        is_valid = is_valid && ValidateUsesArguments(relationship, declarations);
      } else if (rel_ref == PqlTokenType::MODIFIES) {
        is_valid = is_valid && ValidateModifiesArguments(relationship, declarations);
      } else if (rel_ref == PqlTokenType::CALLS) {
        is_valid = is_valid && ValidateCallsCallsTArguments(relationship, declarations);
      } else if (rel_ref == PqlTokenType::CALLS_T) {
        is_valid = is_valid && ValidateCallsCallsTArguments(relationship, declarations);
      } else if (rel_ref == PqlTokenType::NEXT) {
        is_valid = is_valid && ValidateNextNextTArguments(relationship, declarations);
      } else if (rel_ref == PqlTokenType::NEXT_T) {
        is_valid = is_valid && ValidateNextNextTArguments(relationship, declarations);
      } else if (rel_ref == PqlTokenType::AFFECTS) {
        is_valid = is_valid && ValidateAffectsAffectsTArguments(relationship, declarations);
      } else if (rel_ref == PqlTokenType::AFFECTS_T) {
        is_valid = is_valid && ValidateAffectsAffectsTArguments(relationship, declarations);
      } else {
        return false;
      }
    }
  }
  return is_valid;
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

  if (first_arg.type==PqlTokenType::UNDERSCORE) {
    return false;
  }
  if (first_arg.type==PqlTokenType::SYNONYM && second_arg.type==PqlTokenType::SYNONYM) {
    if (declarations.count(first_arg.value) == 0) {
      return false;
    }
    if (declarations.count(second_arg.value) == 0) {
      return false;
    }

    if (first_arg.value==second_arg.value) {
      // Uses(s, s) is semantically invalid
      return false;
    }

    if (!IsStmt(declarations.at(first_arg.value)) && !IsProc(declarations.at(first_arg.value))) {
      return false;
    }

    if (declarations.at(second_arg.value) !=DesignEntityType::VARIABLE) {
      return false;
    }
  } else if (first_arg.type == PqlTokenType::SYNONYM) {
    if (!IsStmt(declarations.at(first_arg.value)) && !IsProc(declarations.at(first_arg.value))) {
      return false;
    }

  } else if (second_arg.type == PqlTokenType::SYNONYM) {
    if (first_arg.type == PqlTokenType::UNDERSCORE) {
      return false;
    }
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
    if (declarations.count(first_arg.value) == 0) {
      return false;
    }
    if (declarations.count(second_arg.value) == 0) {
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
    if (first_arg.value==second_arg.value && relationship.GetRelRef().type == PqlTokenType::NEXT) {
      // Next (s, s) is semantically invalid
      return false;
    }

    if (declarations.count(first_arg.value) == 0) {
      return false;
    }
    if (declarations.count(second_arg.value) == 0) {
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
  bool is_valid = true;
  if (!patterns.empty()) {
    for (auto pattern : patterns) {
      PqlToken pattern_syn = pattern.GetSynonym();
      if (declarations.count(pattern_syn.value) == 0) {
        return false;
      }
      DesignEntityType design_entity_type = declarations.at(pattern_syn.value);
      if (design_entity_type == DesignEntityType::ASSIGN) {
        is_valid = is_valid && ValidateAssignPatternArguments(pattern, declarations);
      } else if (design_entity_type == DesignEntityType::WHILE) {
        is_valid = is_valid && ValidateWhilePatternArguments(pattern, declarations);
      } else if (design_entity_type == DesignEntityType::IF) {
        is_valid = is_valid && ValidateIfPatternArguments(pattern, declarations);
      } else {
        return false;
      }
    }
  }

  return is_valid;
}

bool ParsedQueryValidator::ValidateAssignPatternArguments(Pattern pattern, std::unordered_map<std::string, DesignEntityType> declarations) {
  PqlToken first_arg = pattern.GetFirst();
  PqlToken second_arg = pattern.GetSecond();

  if (!pattern.GetThird().value.empty()) {
    throw "ERROR: Invalid assign pattern grammar!";
  }

  if (first_arg.type==PqlTokenType::SYNONYM) {
    if (declarations.count(first_arg.value) == 0) {
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

  if (!pattern.GetThird().value.empty()) {
    throw "ERROR: Invalid assign pattern grammar!";
  }

  if (first_arg.type==PqlTokenType::SYNONYM) {
    if (declarations.count(first_arg.value) == 0) {
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

  if (pattern.GetThird().value.empty()) {
    throw std::runtime_error("ERROR: Invalid assign pattern grammar!");
  }

  if (first_arg.type==PqlTokenType::SYNONYM) {
    if (declarations.count(first_arg.value) == 0) {
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

bool ParsedQueryValidator::ValidateWithClause(ParsedQuery query) {
  std::vector<With> withs = query.GetWithClause();
  std::unordered_map<std::string, DesignEntityType> declarations = query.GetDeclaration();
  for (auto with : withs) {
    PqlToken first_arg = with.GetFirst();
    PqlToken second_arg = with.GetSecond();

    if (first_arg.type == PqlTokenType::ATTRIBUTE && second_arg.type == PqlTokenType::ATTRIBUTE) {
      if (!ValidateAttribute(first_arg, declarations) || !ValidateAttribute(second_arg, declarations)) {
        return false;
      }

      std::pair<std::pair<DesignEntityType, std::string>, AtrriName> first_attribute = Utils::ParseAttributeRef(first_arg, declarations);
      std::pair<std::pair<DesignEntityType, std::string>, AtrriName> second_attribute = Utils::ParseAttributeRef(second_arg, declarations);

      if (IsNameAttribute(first_attribute.second)) {
        if (!IsNameAttribute(second_attribute.second)) {
          return false;
        }
      } else if (IsIntegerAttribute(first_attribute.second)) {
        if (!IsIntegerAttribute(second_attribute.second)) {
          return false;
        }
      }
    } else if (first_arg.type == PqlTokenType::ATTRIBUTE) {
      if (!ValidateAttribute(first_arg, declarations)) {
        return false;
      }

      std::pair<std::pair<DesignEntityType, std::string>, AtrriName> first_attribute = Utils::ParseAttributeRef(first_arg, declarations);

      if (IsNameAttribute(first_attribute.second)) {
        if (second_arg.type != PqlTokenType::IDENT_WITH_QUOTES) {
          return false;
        }
      } else if (IsIntegerAttribute(first_attribute.second)) {
        if (second_arg.type != PqlTokenType::NUMBER) {
          return false;
        }
      }
    } else if (second_arg.type == PqlTokenType::ATTRIBUTE) {
      if (!ValidateAttribute(second_arg, declarations)) {
        return false;
      }
      std::pair<std::pair<DesignEntityType, std::string>, AtrriName> second_attribute = Utils::ParseAttributeRef(second_arg, declarations);

      if (IsNameAttribute(second_attribute.second)) {
        if (first_arg.type != PqlTokenType::IDENT_WITH_QUOTES) {
          return false;
        }
      } else if (IsIntegerAttribute(second_attribute.second)) {
        if (first_arg.type != PqlTokenType::NUMBER) {
          return false;
        }
      }
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

bool ParsedQueryValidator::IsNameAttribute(AtrriName attri_name) {
  return attri_name==AtrriName::PROCNAME
      || attri_name==AtrriName::VARNAME;
}

bool ParsedQueryValidator::IsIntegerAttribute(AtrriName attri_name) {
  return attri_name==AtrriName::VALUE
      || attri_name==AtrriName::STMTNO;
}

}