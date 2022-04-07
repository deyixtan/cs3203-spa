#include "parsed_query_validator.h"

#include <vector>

namespace pql_validator {
std::unordered_set<AttriName> procedure_attribute_names = {
    AttriName::PROCNAME
};

std::unordered_set<AttriName> call_attribute_names = {
    AttriName::PROCNAME,
    AttriName::STMTNO
};

std::unordered_set<AttriName> variable_attribute_names = {
    AttriName::VARNAME
};

std::unordered_set<AttriName> read_and_print_attribute_names = {
    AttriName::VARNAME,
    AttriName::STMTNO
};

std::unordered_set<AttriName> constant_attribute_names = {
    AttriName::VALUE
};

std::unordered_set<AttriName> stmt_attribute_names = {
    AttriName::STMTNO
};

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

std::unordered_set<DesignEntityType> stmt_design_entities = {
    DesignEntityType::STMT,
    DesignEntityType::READ,
    DesignEntityType::PRINT,
    DesignEntityType::CALL,
    DesignEntityType::WHILE,
    DesignEntityType::IF,
    DesignEntityType::ASSIGN
};

std::unordered_set<DesignEntityType> stmt_proc_design_entities = {
    DesignEntityType::STMT,
    DesignEntityType::READ,
    DesignEntityType::PRINT,
    DesignEntityType::CALL,
    DesignEntityType::WHILE,
    DesignEntityType::IF,
    DesignEntityType::ASSIGN,
    DesignEntityType::PROCEDURE
};

std::unordered_set<DesignEntityType> variable_design_entity = {
    DesignEntityType::VARIABLE
};

std::unordered_set<DesignEntityType> procedure_design_entity = {
    DesignEntityType::PROCEDURE
};

std::unordered_set<DesignEntityType> assign_stmt_design_entity = {
    DesignEntityType::ASSIGN,
    DesignEntityType::STMT
};

std::vector<std::unordered_set<DesignEntityType>> stmt_stmt_vector = {
    stmt_design_entities,
    stmt_design_entities
};

std::vector<std::unordered_set<DesignEntityType>> stmt_var_vector = {
    stmt_proc_design_entities,
    variable_design_entity
};

std::vector<std::unordered_set<DesignEntityType>> proc_proc_vector = {
    procedure_design_entity,
    procedure_design_entity
};

std::vector<std::unordered_set<DesignEntityType>> assign_assign_vector = {
    assign_stmt_design_entity,
    assign_stmt_design_entity
};

std::unordered_map<PqlTokenType,
                   std::vector<std::unordered_set<DesignEntityType>>> rel_ref_validation_map = {
    {PqlTokenType::FOLLOWS, stmt_stmt_vector},
    {PqlTokenType::FOLLOWS_T, stmt_stmt_vector},
    {PqlTokenType::PARENT, stmt_stmt_vector},
    {PqlTokenType::PARENT_T, stmt_stmt_vector},
    {PqlTokenType::USES, stmt_var_vector},
    {PqlTokenType::MODIFIES, stmt_var_vector},
    {PqlTokenType::CALLS, proc_proc_vector},
    {PqlTokenType::CALLS_T, proc_proc_vector},
    {PqlTokenType::NEXT, stmt_stmt_vector},
    {PqlTokenType::NEXT_T, stmt_stmt_vector},
    {PqlTokenType::AFFECTS, assign_assign_vector},
    {PqlTokenType::AFFECTS_T, assign_assign_vector}
};

std::unordered_set<PqlTokenType> invalid_underscore_rel_ref = {
    PqlTokenType::USES,
    PqlTokenType::MODIFIES
};

bool ParsedQueryValidator::IsQuerySemanticallyValid(ParsedQuery query) {
  return IsDeclarationSemanticallyValid(query.GetDeclaration())
      && IsResultClauseSemanticallyValid(query)
      && IsRelationshipClauseSemanticallyValid(query)
      && IsPatternClauseSemanticallyValid(query)
      && IsWithClauseSemanticallyValid(query);
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

  std::unordered_set<DesignEntityType> first_arg_set = rel_ref_validation_map.at(rel_ref.type)[0];
  std::unordered_set<DesignEntityType> second_arg_set = rel_ref_validation_map.at(rel_ref.type)[1];

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
    if (!ArePatternArgsSemanticallyValid(pattern, declarations)) {
      return false;
    }
  }
  return true;
}

bool ParsedQueryValidator::ArePatternArgsSemanticallyValid(Pattern pattern, std::unordered_map<std::string, DesignEntityType> declarations) {
  PqlToken pattern_synonym = pattern.GetSynonym();
  PqlToken first_arg = pattern.GetFirst();
  PqlToken second_arg = pattern.GetSecond();
  PqlToken third_arg = pattern.GetThird();
  if (!IsSynonymDeclared(pattern_synonym, declarations)) {
    return false;
  }
  if (first_arg.type == PqlTokenType::SYNONYM) {
    if (!IsSynonymDeclared(first_arg, declarations) || declarations.at(first_arg.value) != DesignEntityType::VARIABLE) {
      return false;
    }
  }
  if (declarations.at(pattern_synonym.value) == DesignEntityType::ASSIGN) {
    return IsPatternAssignSemanticallyValid(pattern);
  } else if (declarations.at(pattern_synonym.value) == DesignEntityType::IF) {
    return IsPatternIfSemanticallyValid(pattern);
  } else if (declarations.at(pattern_synonym.value) == DesignEntityType::WHILE) {
    return IsPatternWhileSemanticallyValid(pattern);
  } else {
    return false;
  }
}

bool ParsedQueryValidator::IsPatternAssignSemanticallyValid(Pattern pattern) {
  PqlToken third_arg = pattern.GetThird();
  if (!third_arg.value.empty()) {
    return false;
  }
  return true;
}

bool ParsedQueryValidator::IsPatternWhileSemanticallyValid(Pattern pattern) {
  PqlToken second_arg = pattern.GetSecond();
  PqlToken third_arg = pattern.GetThird();
  if (second_arg.type != PqlTokenType::UNDERSCORE || !third_arg.value.empty()) {
    return false;
  }
  return true;
}
bool ParsedQueryValidator::IsPatternIfSemanticallyValid(Pattern pattern) {
  PqlToken second_arg = pattern.GetSecond();
  PqlToken third_arg = pattern.GetThird();
  if (second_arg.type != PqlTokenType::UNDERSCORE || third_arg.type != PqlTokenType::UNDERSCORE) {
    return false;
  }
  return true;
}

bool ParsedQueryValidator::IsWithClauseSemanticallyValid(ParsedQuery query) {
  std::vector<With> withs = query.GetWithClause();
  std::unordered_map<std::string, DesignEntityType> declarations = query.GetDeclaration().GetDeclarations();
  bool is_valid = true;
  for (auto with : withs) {
    PqlToken first_arg = with.GetFirst();
    PqlToken second_arg = with.GetSecond();
    if (first_arg.type == PqlTokenType::ATTRIBUTE && second_arg.type == PqlTokenType::ATTRIBUTE) {
      is_valid = is_valid && IsAttributeAttributeSemanticallyValid(first_arg, second_arg, declarations);
    } else if (first_arg.type == PqlTokenType::ATTRIBUTE) {
      is_valid = is_valid && IsAttributeOthersSemanticallyValid(first_arg, second_arg, declarations);
    } else if (second_arg.type == PqlTokenType::ATTRIBUTE) {
      is_valid = is_valid && IsOthersAttributeSemanticallyValid(first_arg, second_arg, declarations);
    }
  }
  return is_valid;
}

bool ParsedQueryValidator::IsAttributeAttributeSemanticallyValid(PqlToken first_arg, PqlToken second_arg, std::unordered_map<std::string, DesignEntityType> declarations) {
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
  return true;
}

bool ParsedQueryValidator::IsAttributeOthersSemanticallyValid(PqlToken first_arg, PqlToken second_arg, std::unordered_map<std::string, DesignEntityType> declarations) {
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
  return true;
}
bool ParsedQueryValidator::IsOthersAttributeSemanticallyValid(PqlToken first_arg, PqlToken second_arg, std::unordered_map<std::string, DesignEntityType> declarations) {
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
  return true;
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