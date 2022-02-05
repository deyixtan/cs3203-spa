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

bool QueryValidator::IsValidRelRefArg(std::vector<PqlToken> rel_ref_tokens, TokenType type) {
  if (type == TokenType::USES) {

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
        if (!IsValidSynonym(declaration_query[j]) {
          throw "ERROR: Invalid Declaration format! \n";
        }
      }
    }
    PqlToken semicolon_token = declaration_query[declaration_query.size() - 1];
    if (semicolon_token.type != TokenType::SEMICOLON) {
      throw "Invalid Declaration format! \n";
    }

    if (contain_select_clause) {
      std::vector<PqlToken> select_clause = token_table[token_table.size() - 1];
      if (select_clause.size() < 2) {
        throw "Invalid Select Clause format! \n";
      }
      PqlToken select_token = select_clause[0];
      if (select_token.type != TokenType::SELECT) {
        throw "Invalid Select keyword! \n";
      }
      // Select v;
      if (select_clause.size() == 2) {
        PqlToken synonym_token = select_clause[1];
        if (!IsValidSynonym(synonym_token)) {
          throw "Invalid synonym to be selected! \n";
        }
      } else {

      }
    }

  }

  return tokens_;
}