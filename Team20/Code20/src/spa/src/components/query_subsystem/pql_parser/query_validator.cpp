#include "query_validator.h"

QueryValidator::QueryValidator(std::vector<PqlToken> tokens) : tokens_(tokens){}

bool QueryValidator::IsValidSynonym(PqlToken synonym_token) {
  // rel_ref or design_entities can also be synonyms
  return synonym_token.type == TokenType::SYNONYM ||
      design_entities.count(synonym_token.type) ||
      rel_ref.count(synonym_token.type);
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
  if (open_parenthesis.type != TokenType::OPEN_PARENTHESIS) {
    return false;
  }
  PqlToken first_arg = rel_ref_tokens[2];
  if (!stmt_ref.count(first_arg.type) && !ent_ref.count(first_arg.type)) {
    return false;
  }
  PqlToken comma = rel_ref_tokens[3];
  if (comma.type != TokenType::COMMA) {
    return false;
  }
  PqlToken second_arg = rel_ref_tokens[4];
  if (!stmt_ref.count(second_arg.type) && !ent_ref.count(second_arg.type)) {
    return false;
  }
  PqlToken close_parenthesis = rel_ref_tokens[5];
  if (close_parenthesis.type != TokenType::CLOSED_PARENTHESIS) {
    return false;
  }
  return true;
}

void QueryValidator::IsValidRelRefArg(std::vector<PqlToken> rel_ref_tokens, TokenType type) {
  PqlToken first_arg = rel_ref_tokens[2];
  PqlToken second_arg = rel_ref_tokens[4];
  if (type == TokenType::USES || type == TokenType::MODIFIES) {
    if (!ent_ref.count(second_arg.type)) {
      throw "ERROR: Invalid arguments for rel ref! \n";
    }
  } else if (type == TokenType::PARENT || type == TokenType::PARENT_T ||
             type == TokenType::FOLLOWS || type == TokenType::FOLLOWS_T) {
    if (!stmt_ref.count(first_arg.type) || !stmt_ref.count(second_arg.type)) {
      throw "ERROR: Invalid arguments for rel ref! \n";
    }
  } else {
    throw "ERROR: Invalid rel ref token! \n";
  }
}

void QueryValidator::IsValidDeclaration(std::vector<PqlToken> declaration_query) {
  if (declaration_query.size() < 3) {
    throw "ERROR: Missing token in declaration! \n";
  }
  PqlToken design_entities_token = declaration_query[0];
  if (!IsValidDesignEntity(design_entities_token)) {
    throw "ERROR: Invalid Design Entity! \n";
  }
  for (int j = 1; j < declaration_query.size() - 1; j++) {
    if (j % 2 == 0) {
      if (declaration_query[j].type != TokenType::COMMA) {
        throw "ERROR: Invalid Declaration format! \n";
      }
    } else {
      if (!IsValidSynonym(declaration_query[j])) {
        throw "ERROR: Invalid Declaration format! \n";
      }
    }
  }
  PqlToken semicolon_token = declaration_query[declaration_query.size() - 1];
  if (semicolon_token.type != TokenType::SEMICOLON) {
    throw "Invalid Declaration format! \n";
  }
}

void QueryValidator::IsValidPatternClause(std::vector<PqlToken> pattern_clause) {
  if (pattern_clause.size() != 7) {
    throw "Invalid Pattern Clause format! \n";
  }
  PqlToken pattern_token = pattern_clause[0];
  if (pattern_token.type != TokenType::PATTERN) {
    throw "Invalid Pattern keyword! \n";
  }

  PqlToken assign_synonym = pattern_clause[1];
  if (!IsValidSynonym(assign_synonym)) {
    throw "Invalid synonym in pattern clause! \n";
  }

  PqlToken open_parenthesis = pattern_clause[2];
  if (open_parenthesis.type != TokenType::OPEN_PARENTHESIS) {
    throw "Invalid Pattern Clause format! \n";
  }

  PqlToken first_arg = pattern_clause[3];
  if (!ent_ref.count(first_arg.type)) {
    throw "Invalid Pattern Clause argument! \n";
  }

  PqlToken comma = pattern_clause[4];
  if (comma.type != TokenType::COMMA) {
    throw "Invalid Pattern Clause format! \n";
  }

  PqlToken second_arg = pattern_clause[5];
  if (!expression_spec.count(second_arg.type)) {
    throw "Invalid Pattern Clause argument! \n";
  }

  PqlToken close_parenthesis = pattern_clause[6];
  if (close_parenthesis.type != TokenType::CLOSED_PARENTHESIS) {
    throw "Invalid Pattern Clause format! \n";
  }
}

void QueryValidator::IsValidSelectClause(std::vector<PqlToken> select_clause) {
  if (select_clause.size() < 2) {
    throw "Invalid Select Clause format! \n";
  }
  PqlToken select_token = select_clause[0];
  if (select_token.type != TokenType::SELECT) {
    throw "Invalid Select keyword! \n";
  }
  // Select v;

  PqlToken synonym_token = select_clause[1];
  if (!IsValidSynonym(synonym_token)) {
    throw "Invalid synonym to be selected! \n";
  }

  if (select_clause.size() >= 2) { // Select s such that Follows (s, s1)
    PqlToken such_token = select_clause[2];
    if (such_token.type != TokenType::SUCH) {
      throw "Invalid Select Clause format! \n";
    }
    PqlToken that_token = select_clause[3];
    if (that_token.type != TokenType::THAT) {
      throw "Invalid Select Clause format! \n";
    }

    std::vector<PqlToken> rel_ref_tokens;
    for (int index = 4; index < 10; index++) {
      rel_ref_tokens.push_back(select_clause[index]);
    }

    if (IsValidRelRefClause(rel_ref_tokens)) {
      IsValidRelRefArg(rel_ref_tokens, rel_ref_tokens[0].type);
    } else {
      throw "ERROR: Invalid rel ref format! \n";
    }
  }

  if (select_clause.size() >= 10) { // Select s such that Follows (s, s1) pattern a (a, "x")
    std::vector<PqlToken> pattern_tokens;
    for (int index = 10; index < select_clause.size() - 1; index++) {
      pattern_tokens.push_back(select_clause[index]);
      IsValidPatternClause(pattern_tokens);
    }
  }
}


std::vector<PqlToken> QueryValidator::CheckValidation() {
  std::vector<std::vector<PqlToken>> token_table;
  std::vector<PqlToken> single_query;
  for (PqlToken single_token : tokens_) {
    single_query.push_back(single_token);
    if (single_token.type == TokenType::SEMICOLON) {
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
      throw "ERROR: Missing declaration or semi-colon! \n";
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
  return tokens_;
}