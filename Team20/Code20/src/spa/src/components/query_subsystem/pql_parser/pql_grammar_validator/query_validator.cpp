#include "query_validator.h"
#include "components/query_subsystem/pql_parser/query_grammar_error.h"

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

void QueryValidator::ReplaceSynonym(int synonym_index) {
  if (allowed_synonyms.count(tokens_[synonym_index].type)) {
    tokens_[synonym_index].type = PqlTokenType::SYNONYM;
  }
}


void QueryValidator::ValidateDeclaration(int declaration_starting_index, int declaration_ended_index) {
  if (declaration_starting_index + MINIMUM_DECLARATION_SIZE > declaration_ended_index) {
    throw MISSING_TOKEN_IN_DECLARATION;
  }
  PqlToken design_entities_token = tokens_[declaration_starting_index];
  if (!IsValidDesignEntity(design_entities_token)) {
    throw INVALID_DESIGN_ENTITY_IN_DECLARATION;
  }
  for (int j = declaration_starting_index + 1; j < declaration_ended_index - 1; j++) {
    if ((j - declaration_starting_index) % 2 == 0) {
      if (tokens_[j].type != PqlTokenType::COMMA) {
        throw INVALID_DECLARATION_FORMAT;
      }
    } else {
      if (!IsValidSynonym(tokens_[j])) {
        throw INVALID_DECLARATION_SYNONYM;
      } else {
        tokens_[j].type = PqlTokenType::SYNONYM;
      }
    }
  }
  PqlToken semicolon_token = tokens_[declaration_ended_index - 1];
  if (semicolon_token.type != PqlTokenType::SEMICOLON) {
    throw INVALID_DECLARATION_FORMAT;
  }
}

void QueryValidator::ValidateResultClause(int result_clause_starting_index) {
  PqlToken result = tokens_[result_clause_starting_index];
  if (IsValidSynonym(result) && result.type != PqlTokenType::BOOLEAN) {
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

  if (rel_ref_first_arg_both_ref.count(rel_ref_token.type)) {
    ReplaceSynonym(first_arg_position);
    if ((!stmt_ref.count(tokens_[first_arg_position].type) &&
            !ent_ref.count(tokens_[first_arg_position].type))) {
      throw INVALID_REL_REF_ARGUMENTS;
    }
  } else if (rel_ref_first_arg_stmt_ref.count(rel_ref_token.type)) {
    ReplaceSynonym(first_arg_position);
    if (!stmt_ref.count(tokens_[first_arg_position].type)) {
      throw INVALID_REL_REF_ARGUMENTS;
    }
  } else if (rel_ref_first_arg_ent_ref.count(rel_ref_token.type)) {
    ReplaceSynonym(first_arg_position);
    if (!ent_ref.count(tokens_[first_arg_position].type)) {
      throw INVALID_REL_REF_ARGUMENTS;
    }
  }
}

void QueryValidator::ValidateRelRefSecondArg(int second_arg_position) {
  PqlToken rel_ref_token = tokens_[second_arg_position - RELATIONSHIP_CLAUSE_SECOND_ARG_POSITION];

  if (rel_ref_second_arg_ent_ref.count(rel_ref_token.type)) {
    ReplaceSynonym(second_arg_position);
    if (!ent_ref.count(tokens_[second_arg_position].type)) {
      throw INVALID_REL_REF_ARGUMENTS;
    }
  } else {
    ReplaceSynonym(second_arg_position);
    if (!stmt_ref.count(tokens_[second_arg_position].type)) {
      throw INVALID_REL_REF_ARGUMENTS;
    }
  }
}

int QueryValidator::ValidateSuchThatClause(int such_that_clause_starting_index) {
  if (such_that_clause_starting_index + SUCH_THAT_WORD_COUNT + RELATIONSHIP_CLAUSE_SIZE > tokens_.size() ||
      tokens_[such_that_clause_starting_index + 1].type != PqlTokenType::THAT) {
    throw INVALID_SELECT_CLAUSE_FORMAT;
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
  if (tokens_[pattern_arg_index].type != PqlTokenType::OPEN_PARENTHESIS) {
    throw INVALID_PATTERN_CLAUSE_FORMAT;
  }
  ReplaceSynonym(pattern_arg_index + PATTERN_CLAUSE_FIRST_ARG_POSITION);
  if (!ent_ref.count(tokens_[pattern_arg_index + PATTERN_CLAUSE_FIRST_ARG_POSITION].type)) {
    throw INVALID_PATTERN_CLAUSE_FORMAT;
  }

  if (tokens_[pattern_arg_index + PATTERN_CLAUSE_CLOSED_PARENTHESIS_POSITION_SHORT].type
      == PqlTokenType::CLOSED_PARENTHESIS) {
    // while and assign pattern

    if (tokens_[pattern_arg_index + PATTERN_CLAUSE_FIRST_COMMA_POSITION].type != PqlTokenType::COMMA) {
      throw INVALID_PATTERN_CLAUSE_FORMAT;
    }

    if (!expression_spec.count(tokens_[pattern_arg_index + PATTERN_CLAUSE_SECOND_ARG_POSITION].type) &&
        tokens_[pattern_arg_index + PATTERN_CLAUSE_SECOND_ARG_POSITION].type != PqlTokenType::UNDERSCORE) {
      throw INVALID_PATTERN_CLAUSE_FORMAT;
    }

    return pattern_arg_index + PATTERN_CLAUSE_CLOSED_PARENTHESIS_POSITION_SHORT + 1;

  } else if (tokens_[pattern_arg_index + PATTERN_CLAUSE_CLOSED_PARENTHESIS_POSITION_LONG].type
      == PqlTokenType::CLOSED_PARENTHESIS) {
    // if pattern
    if (tokens_[pattern_arg_index + PATTERN_CLAUSE_FIRST_COMMA_POSITION].type != PqlTokenType::COMMA ||
        tokens_[pattern_arg_index + PATTERN_CLAUSE_SECOND_COMMA_POSITION].type != PqlTokenType::COMMA) {
      throw INVALID_PATTERN_CLAUSE_FORMAT;
    }

    if (tokens_[pattern_arg_index + PATTERN_CLAUSE_SECOND_ARG_POSITION].type != PqlTokenType::UNDERSCORE ||
        tokens_[pattern_arg_index + PATTERN_CLAUSE_THIRD_ARG_POSITION].type != PqlTokenType::UNDERSCORE) {
      throw INVALID_PATTERN_CLAUSE_FORMAT;
    }

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
    if (current_index == pattern_clause_starting_index) {
      if (!allowed_synonyms.count(tokens_[current_index + 1].type)) {
        throw INVALID_PATTERN_SYNONYM;
      } else {
        tokens_[current_index + 1].type = PqlTokenType::SYNONYM;
      }
      int pattern_arg_index = current_index + PATTERN_CLAUSE_ARG_POSITION;
      current_index = ValidatePatternArg(pattern_arg_index);
    } else if (tokens_[current_index].type == PqlTokenType::AND) {
      // and clause
      if (!allowed_synonyms.count(tokens_[current_index + 1].type)) {
        throw INVALID_PATTERN_SYNONYM;
      } else {
        tokens_[current_index + 1].type = PqlTokenType::SYNONYM;
      }
      int pattern_arg_index = current_index + PATTERN_CLAUSE_ARG_POSITION;
      current_index = ValidatePatternArg(pattern_arg_index);
    } else {
      break;
    }
  }
  return current_index;
}
int QueryValidator::ValidateWithClause(int with_clause_starting_index) {
  if (with_clause_starting_index + WITH_CLAUSE_SIZE > tokens_.size()) {
    throw INVALID_WITH_CLAUSE_FORMAT;
  }
  int current_index = with_clause_starting_index;
  while (current_index < tokens_.size()) {
    if (current_index == with_clause_starting_index || tokens_[current_index].type == PqlTokenType::AND) {
      if (!with_clause_ref.count(tokens_[with_clause_starting_index + WITH_CLAUSE_FIRST_ARG_POSITION].type) ||
          !with_clause_ref.count(tokens_[with_clause_starting_index + WITH_CLAUSE_SECOND_ARG_POSITION].type) ||
          tokens_[with_clause_starting_index + WITH_CLAUSE_EQUAL_POSITION].type != PqlTokenType::EQUAL) {
        throw INVALID_WITH_CLAUSE_FORMAT;
      }
      current_index += WITH_CLAUSE_SIZE;
    } else {
      break;
    }
  }
  return current_index;
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
   | Select a1        |
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
  int declaration_starting_index = 0;
  for (int i = 0; i < last_declaration_index + 1; i++) {
    std::vector<PqlToken> declaration_query = token_table[i];
    ValidateDeclaration(declaration_starting_index, declaration_starting_index + declaration_query.size());
    declaration_starting_index += declaration_query.size();
  }

  // validate select clause if query has select clause
  if (contain_select_clause) {
    int select_clause_length = token_table[token_table.size() - 1].size();
    ValidateSelectClause(tokens_.size() - select_clause_length);
  }
  return tokens_;
}