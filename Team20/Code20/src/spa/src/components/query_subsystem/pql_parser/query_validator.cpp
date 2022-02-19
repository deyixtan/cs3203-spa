#include "query_validator.h"
#include "query_grammar_error.h"

QueryValidator::QueryValidator(std::vector<PqlToken> tokens) : tokens_(tokens){}

bool QueryValidator::IsValidSynonym(PqlToken synonym_token) {
  // rel_ref or design_entities can also be synonyms
  return allowed_synonyms.count(synonym_token.type);
}

bool QueryValidator::IsValidRelRefToken(PqlToken rel_ref_token) {
  return rel_ref.count(rel_ref_token.type);
}

bool QueryValidator::IsValidDesignEntity(PqlToken design_entity_token) {
  return design_entities.count(design_entity_token.type);
}

bool QueryValidator::IsValidRelRefClause(std::vector<PqlToken> rel_ref_tokens) {
  if (rel_ref_tokens.size() != 6) {
    return false;
  }
  PqlToken rel_ref_token = rel_ref_tokens[0];
  if (!IsValidRelRefToken(rel_ref_token)) {
    return false;
  }
  PqlToken open_parenthesis = rel_ref_tokens[1];
  if (open_parenthesis.type != PqlTokenType::OPEN_PARENTHESIS) {
    return false;
  }
  PqlToken first_arg = rel_ref_tokens[2];
  if (!stmt_ref.count(first_arg.type) && !ent_ref.count(first_arg.type) && !allowed_synonyms.count(first_arg.type)) {
    return false;
  }
  PqlToken comma = rel_ref_tokens[3];
  if (comma.type != PqlTokenType::COMMA) {
    return false;
  }
  PqlToken second_arg = rel_ref_tokens[4];
  if (!stmt_ref.count(second_arg.type) && !ent_ref.count(second_arg.type) && !allowed_synonyms.count(second_arg.type)) {
    return false;
  }
  PqlToken close_parenthesis = rel_ref_tokens[5];
  if (close_parenthesis.type != PqlTokenType::CLOSED_PARENTHESIS) {
    return false;
  }
  return true;
}

void QueryValidator::IsValidRelRefArg(std::vector<PqlToken> rel_ref_tokens, PqlTokenType type) {
  PqlToken first_arg = rel_ref_tokens[2];
  PqlToken second_arg = rel_ref_tokens[4];
  if (type == PqlTokenType::USES || type == PqlTokenType::MODIFIES) {
    if (!ent_ref.count(second_arg.type) && !allowed_synonyms.count(second_arg.type)) {
      throw INVALID_REL_REF_ARGUMENTS;
    }
  } else if (type == PqlTokenType::PARENT || type == PqlTokenType::PARENT_T ||
             type == PqlTokenType::FOLLOWS || type == PqlTokenType::FOLLOWS_T) {
    if ((!stmt_ref.count(first_arg.type) && !allowed_synonyms.count(first_arg.type)) ||
        (!stmt_ref.count(second_arg.type) && !allowed_synonyms.count(second_arg.type))) {
      throw INVALID_REL_REF_ARGUMENTS;
    }
  } else {
    throw INVALID_REL_REF_TOKEN;
  }
}

void QueryValidator::IsValidDeclaration(std::vector<PqlToken> declaration_query) {
  if (declaration_query.size() < 3) {
    throw MISSING_TOKEN_IN_DECLARATION;
  }
  PqlToken design_entities_token = declaration_query[0];
  if (!IsValidDesignEntity(design_entities_token)) {
    throw INVALID_DESIGN_ENTITY_IN_DECLARATION;
  }
  for (int j = 1; j < declaration_query.size() - 1; j++) {
    if (j % 2 == 0) {
      if (declaration_query[j].type != PqlTokenType::COMMA) {
        throw INVALID_DECLARATION_FORMAT;
      }
    } else {
      if (!IsValidSynonym(declaration_query[j])) {
        throw INVALID_DECLARATION_SYNONYM;
      } else {
        declaration_query[j].type = PqlTokenType::SYNONYM;
      }
    }
  }
  PqlToken semicolon_token = declaration_query[declaration_query.size() - 1];
  if (semicolon_token.type != PqlTokenType::SEMICOLON) {
    throw INVALID_DECLARATION_FORMAT;
  }
}

void QueryValidator::IsValidPatternClause(std::vector<PqlToken> pattern_clause) {
  if (pattern_clause.size() != 7) {
    throw INVALID_PATTERN_CLAUSE_FORMAT;
  }
  PqlToken pattern_token = pattern_clause[0];
  if (pattern_token.type != PqlTokenType::PATTERN) {
    throw INVALID_PATTERN_KEYWORD;
  }

  PqlToken assign_synonym = pattern_clause[1];
  if (!IsValidSynonym(assign_synonym)) {
    throw INVALID_PATTERN_SYNONYM;
  } else {
    pattern_clause[1].type = PqlTokenType::SYNONYM;
  }

  PqlToken open_parenthesis = pattern_clause[2];
  if (open_parenthesis.type != PqlTokenType::OPEN_PARENTHESIS) {
    throw INVALID_PATTERN_CLAUSE_FORMAT;
  }

  PqlToken first_arg = pattern_clause[3];
  if (!ent_ref.count(first_arg.type) && !allowed_synonyms.count(first_arg.type)) {
    throw INVALID_PATTERN_CLAUSE_ARGUMENT;
  }

  PqlToken comma = pattern_clause[4];
  if (comma.type != PqlTokenType::COMMA) {
    throw INVALID_PATTERN_CLAUSE_FORMAT;
  }

  PqlToken second_arg = pattern_clause[5];
  if (!expression_spec.count(second_arg.type)) {
    throw INVALID_PATTERN_CLAUSE_ARGUMENT;
  }

  PqlToken close_parenthesis = pattern_clause[6];
  if (close_parenthesis.type != PqlTokenType::CLOSED_PARENTHESIS) {
    throw INVALID_PATTERN_CLAUSE_FORMAT;
  }
}

void QueryValidator::IsValidSelectClause(std::vector<PqlToken> select_clause) {
  if (select_clause.size() < 2) {
    throw INVALID_SELECT_CLAUSE_FORMAT;
  }
  PqlToken select_token = select_clause[0];
  if (select_token.type != PqlTokenType::SELECT) {
    throw INVALID_SELECT_KEYWORD;
  }
  // Select v;

  PqlToken synonym_token = select_clause[1];
  if (!IsValidSynonym(synonym_token)) {
    throw INVALID_SELECT_SYNONYM;
  } else {
    select_clause[1].type = PqlTokenType::SYNONYM;
  }

  if (select_clause.size() > 2) { // Select s such that Follows (s, s1)

    PqlToken third_token = select_clause[2];
    if (third_token.type == PqlTokenType::SUCH) {  // such that
      PqlToken that_token = select_clause[3];
      if (that_token.type != PqlTokenType::THAT) {
        throw INVALID_SELECT_CLAUSE_FORMAT;
      }
      std::vector<PqlToken> rel_ref_tokens;
      for (int index = 4; index < 10; index++) {
        rel_ref_tokens.push_back(select_clause[index]);
      }
      if (IsValidRelRefClause(rel_ref_tokens)) {
        IsValidRelRefArg(rel_ref_tokens, rel_ref_tokens[0].type);
      } else {
        throw INVALID_REL_REF_FORMAT;
      }
    } else if (third_token.type == PqlTokenType::PATTERN) {
      std::vector<PqlToken> pattern_tokens;
      for (int index = 2; index < select_clause.size(); index++) {
        pattern_tokens.push_back(select_clause[index]);
      }
      IsValidPatternClause(pattern_tokens);
    } else {
      throw INVALID_SELECT_CLAUSE_FORMAT;
    }
  }

  if (select_clause.size() > 10) { // Select s such that Follows (s, s1) pattern a (a, "x")
    std::vector<PqlToken> pattern_tokens;
    for (int index = 10; index < select_clause.size(); index++) {
      pattern_tokens.push_back(select_clause[index]);
    }
    IsValidPatternClause(pattern_tokens);
  }
}


std::vector<PqlToken> QueryValidator::CheckValidation() {
  std::vector<std::vector<PqlToken>> token_table;
  std::vector<PqlToken> single_query;
  for (PqlToken single_token : tokens_) {
    single_query.push_back(single_token);
    if (single_token.type == PqlTokenType::SEMICOLON) {
      token_table.push_back(single_query);
      single_query.clear();
    }
  }
  bool contain_select_clause = false;
  if (!single_query.empty()) {
    token_table.push_back(single_query);
    contain_select_clause = true;
  }

  int last_declaration_index;
  if (contain_select_clause) {
    if (token_table.size() < 2) {
      throw MISSING_TOKEN_IN_DECLARATION;
    }
    last_declaration_index = token_table.size() - 2;
  } else {
    last_declaration_index = token_table.size() - 1;
  }

  // check declaration query (excluding the last vector in the table)
  for (int i = 0; i < last_declaration_index + 1; i++) {
    std::vector<PqlToken> declaration_query = token_table[i];
    IsValidDeclaration(declaration_query);
  }

  if (contain_select_clause) {
    std::vector<PqlToken> select_clause = token_table[token_table.size() - 1];
    IsValidSelectClause(select_clause);
  }
  ConvertTokenType(tokens_);
  return tokens_;
}

void QueryValidator::ConvertTokenType(std::vector<PqlToken> &tokens) {
  int curr = 1;
  int last_semicolon_index;

  while (curr < tokens.size() - 1) {
    if (tokens[curr].type == PqlTokenType::SEMICOLON) {
      tokens[curr - 1].type = PqlTokenType::SYNONYM;
      last_semicolon_index = curr;
    }
    curr++;
  }

  if (tokens.size() - last_semicolon_index > 0) {
    tokens[last_semicolon_index + 2].type = PqlTokenType::SYNONYM;
  }

  if (tokens.size() - last_semicolon_index > 2) {
    if (tokens[last_semicolon_index + 3].type == PqlTokenType::SUCH) {
      if (tokens[last_semicolon_index + 7].type != PqlTokenType::UNDERSCORE &&
          tokens[last_semicolon_index + 7].type != PqlTokenType::IDENT_WITH_QUOTES &&
          tokens[last_semicolon_index + 7].type != PqlTokenType::NUMBER) {
        tokens[last_semicolon_index + 7].type = PqlTokenType::SYNONYM;
      }
      if (tokens[last_semicolon_index + 9].type != PqlTokenType::UNDERSCORE &&
          tokens[last_semicolon_index + 9].type != PqlTokenType::IDENT_WITH_QUOTES &&
          tokens[last_semicolon_index + 9].type != PqlTokenType::NUMBER) {
        tokens[last_semicolon_index + 9].type = PqlTokenType::SYNONYM;
      }
    } else {
      tokens[last_semicolon_index + 4].type = PqlTokenType::SYNONYM;
      if (tokens[last_semicolon_index + 6].type != PqlTokenType::UNDERSCORE &&
          tokens[last_semicolon_index + 6].type != PqlTokenType::IDENT_WITH_QUOTES) {
        tokens[last_semicolon_index + 6].type = PqlTokenType::SYNONYM;
      }
    }
  }
  if (tokens.size() - last_semicolon_index > 10) {
    tokens[last_semicolon_index + 12].type = PqlTokenType::SYNONYM;
    if (tokens[last_semicolon_index + 14].type != PqlTokenType::UNDERSCORE &&
        tokens[last_semicolon_index + 14].type != PqlTokenType::IDENT_WITH_QUOTES) {
      tokens[last_semicolon_index + 14].type = PqlTokenType::SYNONYM;
    }
  }
}