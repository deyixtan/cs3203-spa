#include "parsed_query_validator.h"

#include <string>
#include <vector>

namespace pql_validator {

std::unordered_map<DesignEntityType, std::unordered_set<AttriName>> attribute_validation_map = {
    {DesignEntityType::STMT, stmt_attribute_names},
    {DesignEntityType::READ, read_and_print_attribute_names},
    {DesignEntityType::PRINT, read_and_print_attribute_names},
    {DesignEntityType::CALL, call_attribute_names},
    {DesignEntityType::WHILE, stmt_attribute_names},
    {DesignEntityType::IF, stmt_attribute_names},
    {DesignEntityType::ASSIGN, stmt_attribute_names},
    {DesignEntityType::VARIABLE, variable_attribute_names},
    {DesignEntityType::CONSTANT, constant_attribute_names},
    {DesignEntityType::PROCEDURE, procedure_attribute_names}
};

std::unordered_map<PqlTokenType,
std::pair<std::unordered_set<DesignEntityType>,
std::unordered_set<DesignEntityType>>> rel_ref_validation_map = {
    {PqlTokenType::FOLLOWS, std::make_pair(stmt_design_entities, stmt_design_entities)},
    {PqlTokenType::FOLLOWS_T, std::make_pair(stmt_design_entities, stmt_design_entities)},
    {PqlTokenType::PARENT, std::make_pair(stmt_design_entities, stmt_design_entities)},
    {PqlTokenType::PARENT_T, std::make_pair(stmt_design_entities, stmt_design_entities)},
    {PqlTokenType::USES, std::make_pair(stmt_design_entities, variable_design_entity)},
    {PqlTokenType::MODIFIES, std::make_pair(stmt_design_entities, variable_design_entity)},
    {PqlTokenType::CALLS, std::make_pair(procedure_design_entity, procedure_design_entity)},
    {PqlTokenType::CALLS_T, std::make_pair(procedure_design_entity, procedure_design_entity)},
    {PqlTokenType::NEXT, std::make_pair(stmt_design_entities, stmt_design_entities)},
    {PqlTokenType::NEXT_T, std::make_pair(stmt_design_entities, stmt_design_entities)},
    {PqlTokenType::AFFECTS, std::make_pair(assign_design_entity, assign_design_entity)},
    {PqlTokenType::AFFECTS_T, std::make_pair(assign_design_entity, assign_design_entity)}
};

std::unordered_set<PqlTokenType> invalid_underscore_rel_ref = {
    PqlTokenType::USES,
    PqlTokenType::MODIFIES
};

bool ParsedQueryValidator::IsQuerySemanticallyValid(ParsedQuery query) {
  return IsDeclarationSemanticallyValid(query.GetDeclaration())
      && IsResultClauseSemanticallyValid(query)
      && IsRelationshipClauseSemanticallyValid(query)
      && ValidatePatternClause(query)
      && ValidateWithClause(query);
}

bool ParsedQueryValidator::IsDeclarationSemanticallyValid(Declaration declaration) {
  if (declaration.HasDuplicatedSynonyms()) {
    return false;
  }
  return true;
}

bool ParsedQueryValidator::IsResultClauseSemanticallyValid(ParsedQuery query) {
  std::unordered_map<std::string, DesignEntityType> declarations = query.GetDeclaration().GetDeclarations();
  ResultClause result_clause = query.GetResultClause();
  bool is_valid = true;
  for (auto token : result_clause.GetValues()) {
    if (token.type == PqlTokenType::SYNONYM) {
      is_valid = is_valid && IsSynonymDeclared(token, declarations);
    } else if (token.type == PqlTokenType::ATTRIBUTE) {
      is_valid = is_valid && IsAttributeSemanticallyValid(token, declarations);
    }
  }
  return is_valid;
}

bool ParsedQueryValidator::IsSynonymDeclared(PqlToken token, std::unordered_map<std::string, DesignEntityType> declarations) {
  if (declarations.count(token.value)) {
    return true;
  }
  return false;
}

bool ParsedQueryValidator::IsAttributeDeclared(std::pair<std::string, AttriName> parsed_attribute, std::unordered_map<std::string, DesignEntityType> declarations) {
  if (declarations.count(parsed_attribute.first)) {
    return true;
  }
  return false;
}

bool ParsedQueryValidator::IsAttributeSemanticallyValid(PqlToken token, std::unordered_map<std::string, DesignEntityType> declarations) {
  std::pair<std::string, AttriName> parsed_attribute_raw = Utils::ParseAttributeRef(token);
  if (!IsAttributeDeclared(parsed_attribute_raw, declarations)) {
    return false;
  }
  std::pair<std::pair<DesignEntityType, std::string>, AttriName> parsed_attribute = Utils::ParseAttributeRef(token, declarations);
  AttriName attri_name = parsed_attribute.second;
  if (!attribute_validation_map.at(parsed_attribute.first.first).count(attri_name)) {
    return false;
  }
  return true;
}



bool ParsedQueryValidator::IsRelationshipClauseSemanticallyValid(ParsedQuery query) {
  std::unordered_map<std::string, DesignEntityType> declarations = query.GetDeclaration().GetDeclarations();

  for (auto relationship : query.GetRelationships()) {
    PqlToken rel_ref_token = relationship.GetRelRef();
    PqlToken first_arg = relationship.GetFirst();
    PqlToken second_arg = relationship.GetSecond();
    if (!AreRelationshipArgsDeclared(first_arg, second_arg, declarations)) {
      return false;
    }
    if (invalid_underscore_rel_ref.count(rel_ref_token.type) && first_arg.type == PqlTokenType::UNDERSCORE) {
      return false;
    }
    if (!AreRelationshipArgsSemanticallyValid(relationship, declarations)) {
      return false;
    }
  }
  return true;
}

bool ParsedQueryValidator::AreRelationshipArgsSemanticallyValid(Relationship relationship, std::unordered_map<std::string, DesignEntityType> declarations) {
  PqlToken rel_ref = relationship.GetRelRef();
  PqlToken first_arg = relationship.GetFirst();
  PqlToken second_arg = relationship.GetSecond();

  std::unordered_set<DesignEntityType> first_arg_set = rel_ref_validation_map.at(rel_ref.type).first;
  std::unordered_set<DesignEntityType> second_arg_set = rel_ref_validation_map.at(rel_ref.type).second;

  if (first_arg.type == PqlTokenType::SYNONYM && !first_arg_set.count(declarations.at(first_arg.value))) {
    return false;
  }
  if (second_arg.type == PqlTokenType::SYNONYM && !second_arg_set.count(declarations.at(second_arg.value))) {
    return false;
  }
  return true;
}

bool ParsedQueryValidator::AreRelationshipArgsDeclared(PqlToken first_arg, PqlToken second_arg, std::unordered_map<std::string, DesignEntityType> declarations) {
  if (first_arg.type == PqlTokenType::SYNONYM && !IsSynonymDeclared(first_arg, declarations)) {
      return false;
  }
  if (second_arg.type == PqlTokenType::SYNONYM && !IsSynonymDeclared(second_arg, declarations)) {
    return false;
  }
  return true;
}

bool ParsedQueryValidator::IsPatternClauseSemanticallyValid(ParsedQuery query) {
  std::vector<Pattern> patterns = query.GetPatterns();
  std::unordered_map<std::string, DesignEntityType> declarations = query.GetDeclaration().GetDeclarations();
  for (auto pattern : patterns) {

  }
  return true;
}

bool ParsedQueryValidator::ArePatternArgsSemanticallyValid(Pattern pattern) {
  PqlToken pattern_synonym = pattern.GetSynonym();
  PqlToken first_arg = pattern.GetFirst();
  PqlToken second_arg = pattern.GetSecond();
  PqlToken third_arg = pattern.GetThird();
}

bool ParsedQueryValidator::ValidatePatternClause(ParsedQuery query) {
  std::vector<Pattern> patterns = query.GetPatterns();
  std::unordered_map<std::string, DesignEntityType> declarations = query.GetDeclaration().GetDeclarations();
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
  std::unordered_map<std::string, DesignEntityType> declarations = query.GetDeclaration().GetDeclarations();
  for (auto with : withs) {
    PqlToken first_arg = with.GetFirst();
    PqlToken second_arg = with.GetSecond();
    if (first_arg.type == PqlTokenType::ATTRIBUTE && second_arg.type == PqlTokenType::ATTRIBUTE) {
      if (!IsAttributeSemanticallyValid(first_arg, declarations) || !IsAttributeSemanticallyValid(second_arg, declarations)) {
        return false;
      }
      std::pair<std::pair<DesignEntityType, std::string>, AttriName> first_attribute = Utils::ParseAttributeRef(first_arg, declarations);
      std::pair<std::pair<DesignEntityType, std::string>, AttriName> second_attribute = Utils::ParseAttributeRef(second_arg, declarations);
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
      if (!IsAttributeSemanticallyValid(first_arg, declarations)) {
        return false;
      }
      std::pair<std::pair<DesignEntityType, std::string>, AttriName> first_attribute = Utils::ParseAttributeRef(first_arg, declarations);
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
      if (!IsAttributeSemanticallyValid(second_arg, declarations)) {
        return false;
      }
      std::pair<std::pair<DesignEntityType, std::string>, AttriName> second_attribute = Utils::ParseAttributeRef(second_arg, declarations);
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

bool ParsedQueryValidator::IsNameAttribute(AttriName attri_name) {
  return attri_name==AttriName::PROCNAME
      || attri_name==AttriName::VARNAME;
}

bool ParsedQueryValidator::IsIntegerAttribute(AttriName attri_name) {
  return attri_name==AttriName::VALUE
      || attri_name==AttriName::STMTNO;
}

}