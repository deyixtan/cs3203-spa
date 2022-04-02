#include "pql_parser.h"

PqlParser::PqlParser(std::vector<PqlToken> tokens) : tokens (tokens), cursor(0), pq(pq) {}

void PqlParser::MoveCursor(int movement) {
  cursor += movement;
}

PqlToken PqlParser::FetchToken() {
  if (cursor < tokens.size()) {
    return tokens[cursor];
  } else {
    throw INVALID_QUERY_FORMAT;
  }
}

PqlToken PqlParser::FetchNextToken() {
  if (cursor + 1 < tokens.size()) {
    return tokens[cursor + 1];
  } else {
    throw INVALID_QUERY_FORMAT;
  }
}

PqlToken PqlParser::ValidateToken(std::unordered_set<PqlTokenType> allowed_types) {
  PqlToken token = FetchToken();
  if (!allowed_types.count(token.type)) {
    throw INVALID_QUERY_FORMAT;
  }
  MoveCursor(1);
  return token;
}

PqlToken PqlParser::ValidateToken(PqlTokenType allowed_type) {
  PqlToken token = FetchToken();
  if (token.type != allowed_type) {
    throw INVALID_QUERY_FORMAT;
  }
  MoveCursor(1);
  return token;
}

int PqlParser::GetEndOfDeclarationCursor() {
  int last_semicolon;
  for (int i = 0; i < tokens.size(); i++) {
    if (tokens[i].type == PqlTokenType::SEMICOLON) {
      last_semicolon = i;
    }
  }
  if (!last_semicolon) {
    throw INVALID_QUERY_FORMAT;
  }
  return last_semicolon;
}

void PqlParser::ParseDeclaration() {
  Declaration declarations;
  int after_declaration_cursor = GetEndOfDeclarationCursor() + 1;
  while (cursor < after_declaration_cursor) {
    PqlToken design_entity_token = ValidateToken(design_entities);
    DesignEntityType design_entity_type = token_design_map[design_entity_token.type];
    ParseDeclarationVariables(design_entity_type, declarations);
  }
  if (declarations.GetDeclarations().empty()) {
    throw INVALID_QUERY_FORMAT;
  }
  pq.SetDeclarations(declarations);
}

void PqlParser::ParseDeclarationVariables(DesignEntityType& design_entity_type, Declaration& declarations) {
  PqlToken next_token = FetchNextToken();
  int starting_variable_index = cursor;
  while (next_token.type != PqlTokenType::SEMICOLON) {
    if ((cursor - starting_variable_index) % 2 == 0) {
      PqlToken synonym_token = ValidateToken(allowed_synonyms);
      SetDuplicatedSynonyms(declarations, synonym_token.value);
      declarations.AddDeclaration(synonym_token.value, design_entity_type);
    } else {
      ValidateToken(PqlTokenType::COMMA);
    }
    next_token = FetchToken();
  }
  MoveCursor(1);
}

void PqlParser::SetDuplicatedSynonyms(Declaration& declarations, std::string token_value) {
  if (declarations.GetDeclarations().count(token_value)) {
    declarations.SetDuplicatedSynonyms();
  }
}

bool PqlParser::HasBooleanSynonym() {
  std::unordered_map<std::string, DesignEntityType> declarations = pq.GetDeclaration().GetDeclarations();
  if (declarations.count(BOOLEAN_SYNONYM_VALUE)) {
    return true;
  }
  return false;
}

void PqlParser::ParseSelectClause() {
  ValidateToken(PqlTokenType::SELECT);
  ParseResultClause();
  while (cursor < tokens.size()) {
    PqlToken current_token = FetchToken();
    if (current_token.type == PqlTokenType::SUCH) {
      ParseResultClause();
    } else if (current_token.type == PqlTokenType::PATTERN) {
      ParsePatternClause();
    } else if (current_token.type == PqlTokenType::WITH) {
      ParseWithClause();
    } else {
      throw INVALID_QUERY_FORMAT;
    }
  }
}

void PqlParser::ParseResultClause() {
  PqlToken result_clause_token = ValidateToken(result_cl);
  ResultClause result_clause(token_result_map[result_clause_token.type]);
  if (result_clause_token.type == PqlTokenType::TUPLE) {
    ParseTupleResultClause(result_clause, result_clause_token);
  } else if (result_clause_token.type == PqlTokenType::BOOLEAN) {
    ParseBooleanResultClause(result_clause, result_clause_token);
  } else {
    ParseSynonymResultClause(result_clause, result_clause_token);
  }
  pq.SetResultClause(result_clause);
}

void PqlParser::ParseSynonymResultClause(ResultClause& result_clause, PqlToken& token) {
  result_clause.AddValue(token);
}

void PqlParser::ParseBooleanResultClause(ResultClause& result_clause, PqlToken& token) {
  if (HasBooleanSynonym()) {
    result_clause.ModifyType(token_result_map[PqlTokenType::SYNONYM]);
    result_clause.AddValue(PqlToken(PqlTokenType::SYNONYM, BOOLEAN_SYNONYM_VALUE));
  } else {
    result_clause.AddValue(token);
  }
}
void PqlParser::ParseTupleResultClause(ResultClause& result_clause, PqlToken& token) {
  std::string trimmed_tuple = token.value.substr(1, token.value.size() - 2);
  std::vector<std::string> token_vector = Utils::SplitString(trimmed_tuple, ',');
  for (std::string single_token : token_vector) {
    if (Utils::ContainsDot(single_token)) {
      result_clause.AddValue(PqlToken(PqlTokenType::ATTRIBUTE, single_token));
    } else {
      result_clause.AddValue(PqlToken(PqlTokenType::SYNONYM, single_token));
    }
  }
}

ParsedQuery PqlParser::ParseQuery() {
  ParseDeclaration();
  ParseSelectClause();
}
















