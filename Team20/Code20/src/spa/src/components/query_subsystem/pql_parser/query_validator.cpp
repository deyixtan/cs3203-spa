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

void QueryValidator::ValidateDeclaration(std::vector<PqlToken> declaration_query) {
  if (declaration_query.size() < MINIMUM_DECLARATION_SIZE) {
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

void QueryValidator::ValidateResultClause(int result_clause_starting_index) {
  PqlToken result = tokens_[result_clause_starting_index];
  if (IsValidSynonym(result)) {
    tokens_[result_clause_starting_index].type = PqlTokenType::SYNONYM;
  } else if (!result_cl_excluding_synonym.count(result.type)) {
    throw INVALID_SELECT_RESULT;
  }
}

void QueryValidator::ValidateRelRefClause(int rel_ref_clause_starting_index) {
  PqlToken rel_ref_token = tokens_[rel_ref_clause_starting_index];
  if (!IsValidRelRefToken(rel_ref_token)) {
    throw INVALID_REL_REF_TOKEN;
  }
  if (tokens_[rel_ref_clause_starting_index + RELATIONSHIP_CLAUSE_OPEN_PARENTHESIS_POSITION].type !=
      PqlTokenType::OPEN_PARENTHESIS ||
      tokens_[rel_ref_clause_starting_index + RELATIONSHIP_CLAUSE_COMMA_POSITION].type != PqlTokenType::COMMA ||
      tokens_[rel_ref_clause_starting_index + RELATIONSHIP_CLAUSE_CLOSED_PARENTHESIS_POSITION].type !=
      PqlTokenType::CLOSED_PARENTHESIS) {
    throw INVALID_REL_REF_FORMAT;
  }
  // check first_argument
  int first_arg_position = rel_ref_clause_starting_index + RELATIONSHIP_CLAUSE_FIRST_ARG_POSITION;
  ValidateRelRefFirstArg(first_arg_position);
  // check second_argument
  int second_arg_position = rel_ref_clause_starting_index + RELATIONSHIP_CLAUSE_SECOND_ARG_POSITION;
  ValidateRelRefSecondArg(second_arg_position);
}

void QueryValidator::ValidateRelRefFirstArg(int first_arg_position) {
  PqlToken rel_ref_token = tokens_[first_arg_position - RELATIONSHIP_CLAUSE_FIRST_ARG_POSITION];

  if (rel_ref_token.type == PqlTokenType::USES || rel_ref_token.type == PqlTokenType::MODIFIES) {
    if (allowed_synonyms.count(tokens_[first_arg_position].type)) {
      tokens_[first_arg_position].type = PqlTokenType::SYNONYM;
    } else if (tokens_[first_arg_position].type == PqlTokenType::UNDERSCORE ||
        (!stmt_ref.count(tokens_[first_arg_position].type) &&
            !ent_ref.count(tokens_[first_arg_position].type))) {
      throw INVALID_REL_REF_ARGUMENTS;
    }
  } else if (rel_ref_token.type == PqlTokenType::FOLLOWS ||
      rel_ref_token.type == PqlTokenType::FOLLOWS_T ||
      rel_ref_token.type == PqlTokenType::PARENT ||
      rel_ref_token.type == PqlTokenType::PARENT_T ||
      rel_ref_token.type == PqlTokenType::NEXT ||
      rel_ref_token.type == PqlTokenType::NEXT_T ||
      rel_ref_token.type == PqlTokenType::AFFECTS ||
      rel_ref_token.type == PqlTokenType::AFFECTS_T) {
    if (allowed_synonyms.count(tokens_[first_arg_position].type)) {
      tokens_[first_arg_position].type = PqlTokenType::SYNONYM;
    } else if (!stmt_ref.count(tokens_[first_arg_position].type)) {
      throw INVALID_REL_REF_ARGUMENTS;
    }
  } else if (rel_ref_token.type == PqlTokenType::CALLS ||
      rel_ref_token.type == PqlTokenType::CALLS_T) {
    if (allowed_synonyms.count(tokens_[first_arg_position].type)) {
      tokens_[first_arg_position].type = PqlTokenType::SYNONYM;
    } else if (!ent_ref.count(tokens_[first_arg_position].type)) {
      throw INVALID_REL_REF_ARGUMENTS;
    }
  }
}

void QueryValidator::ValidateRelRefSecondArg(int second_arg_position) {
  PqlToken rel_ref_token = tokens_[second_arg_position - RELATIONSHIP_CLAUSE_SECOND_ARG_POSITION];

  if (rel_ref_token.type == PqlTokenType::USES ||
      rel_ref_token.type == PqlTokenType::MODIFIES ||
      rel_ref_token.type == PqlTokenType::CALLS ||
      rel_ref_token.type == PqlTokenType::CALLS_T) {
    if (allowed_synonyms.count(tokens_[second_arg_position].type)) {
      tokens_[second_arg_position].type = PqlTokenType::SYNONYM;
    } else if (!ent_ref.count(tokens_[second_arg_position].type)) {
      throw INVALID_REL_REF_ARGUMENTS;
    }
  } else {
    if (allowed_synonyms.count(tokens_[second_arg_position].type)) {
      tokens_[second_arg_position].type = PqlTokenType::SYNONYM;
    } else if (!stmt_ref.count(tokens_[second_arg_position].type)) {
      throw INVALID_REL_REF_ARGUMENTS;
    }
  }
}

int QueryValidator::ValidateSuchThatClause(int such_that_clause_starting_index) {
  if (such_that_clause_starting_index + SUCH_THAT_WORD_COUNT + RELATIONSHIP_CLAUSE_SIZE > tokens_.size() ||
      tokens_[such_that_clause_starting_index + 1].type != PqlTokenType::THAT) {
    throw INVALID_REL_REF_FORMAT;
  }

  int current_index = such_that_clause_starting_index;
  while (current_index < tokens_.size()) {
    if (current_index == such_that_clause_starting_index) {
      int rel_ref_clause_starting_index = current_index + SUCH_THAT_WORD_COUNT;
      ValidateRelRefClause(rel_ref_clause_starting_index);
      current_index += SUCH_THAT_WORD_COUNT + RELATIONSHIP_CLAUSE_SIZE;
    } else if (tokens_[current_index].type == PqlTokenType::AND) {
      // and clause
      int rel_ref_clause_starting_index = current_index + 1;
      ValidateRelRefClause(rel_ref_clause_starting_index);
      current_index += 1 + RELATIONSHIP_CLAUSE_SIZE;
    } else {
      break;
    }
  }
  return current_index;
}

int QueryValidator::ValidatePatternArg(int pattern_arg_index) {
  if (tokens_[pattern_arg_index + PATTERN_CLAUSE_CLOSED_PARENTHESIS_POSITION_SHORT].type
      == PqlTokenType::CLOSED_PARENTHESIS) {
    // while and assign pattern
    // TODO
    return pattern_arg_index + PATTERN_CLAUSE_CLOSED_PARENTHESIS_POSITION_SHORT + 1;

  } else if (tokens_[pattern_arg_index + PATTERN_CLAUSE_CLOSED_PARENTHESIS_POSITION_LONG].type
      == PqlTokenType::CLOSED_PARENTHESIS) {
    // if pattern
    // TODO
    return pattern_arg_index + PATTERN_CLAUSE_CLOSED_PARENTHESIS_POSITION_LONG + 1;
  } else {
    throw INVALID_PATTERN_CLAUSE_FORMAT;
  }
}

int QueryValidator::ValidatePatternClause(int pattern_clause_starting_index) {
  if (pattern_clause_starting_index + 1 + PATTERN_CLAUSE_SIZE_SHORT > tokens_.size()) {
    throw INVALID_PATTERN_CLAUSE_FORMAT;
  }
  if (tokens_[pattern_clause_starting_index].type != PqlTokenType::PATTERN) {
    throw INVALID_PATTERN_KEYWORD;
  }
  int current_index = pattern_clause_starting_index;
  while (current_index < tokens_.size()) {
    if (!allowed_synonyms.count(tokens_[current_index + 1].type)) {
      throw INVALID_PATTERN_SYNONYM;
    }
    if (current_index == pattern_clause_starting_index) {
      int pattern_arg_index = current_index + PATTERN_CLAUSE_ARG_POSITION;
      current_index = ValidatePatternArg(pattern_arg_index);
    } else if (tokens_[current_index].type == PqlTokenType::AND) {
      // and clause
      int pattern_arg_index = current_index + PATTERN_CLAUSE_ARG_POSITION;
      current_index = ValidatePatternArg(pattern_arg_index);
    } else {
      break;
    }
  }
  return current_index;
}
int QueryValidator::ValidateWithClause(int with_clause_starting_index) {
  // TODO
}

void QueryValidator::ValidateSelectClause(int select_clause_starting_index) {
  int select_clause_size = tokens_.size() - select_clause_starting_index;
  if (select_clause_size < MINIMUM_SELECT_CLAUSE_SIZE) {
    throw INVALID_SELECT_CLAUSE_FORMAT;
  }

  PqlToken select_token = tokens_[select_clause_starting_index];
  if (select_token.type != PqlTokenType::SELECT) {
    throw INVALID_SELECT_KEYWORD;
  }

  // Result clause;
  int result_clause_starting_index = select_clause_starting_index + 1;
  ValidateResultClause(result_clause_starting_index);

  // (suchthat-cl | pattern-cl | with-cl)*
  int current_checking_index = select_clause_starting_index + MINIMUM_SELECT_CLAUSE_SIZE;

  while (current_checking_index < tokens_.size()) {
    PqlToken first_clause_token = tokens_[current_checking_index];
    switch (first_clause_token.type) {
      case PqlTokenType::SUCH: {
        current_checking_index = ValidateSuchThatClause(current_checking_index);
        break;
      }
      case PqlTokenType::PATTERN: {
        current_checking_index = ValidatePatternClause(current_checking_index);
        break;
      }
      case PqlTokenType::WITH: {
        current_checking_index = ValidateWithClause(current_checking_index);
        break;
      }
      default: {
        throw INVALID_SELECT_CLAUSE_FORMAT;
      }
    }
  }
}


std::vector<PqlToken> QueryValidator::CheckValidation() {
  // populate token table
  // assign a1, a2; variable v1; Select a1
  /*
   ____________________
   | assign a1 , a2 ; |
   ____________________
   | variable v1 ;    |
   ____________________
   | Select a1;       |
   ____________________
   */

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
    if (token_table.size() < MINIMUM_TOKEN_TABLE_SIZE_FOR_SELECT_CLAUSE) {
      throw MISSING_TOKEN_IN_DECLARATION;
    }
    last_declaration_index = token_table.size() - MINIMUM_TOKEN_TABLE_SIZE_FOR_SELECT_CLAUSE;
  } else {
    last_declaration_index = token_table.size() - 1;
  }

  // validate declaration query (excluding the last vector in the table)
  for (int i = 0; i < last_declaration_index + 1; i++) {
    std::vector<PqlToken> declaration_query = token_table[i];
    ValidateDeclaration(declaration_query);
  }

  // validate select clause if query has select clause
  if (contain_select_clause) {
    int select_clause_length = token_table[token_table.size() - 1].size();
    ValidateSelectClause(tokens_.size() - select_clause_length);
  }
  // TODO: delete this method
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