#include "pql_parser.h"

using namespace pql;

PqlParser::PqlParser(std::vector<PqlToken> tokens) : tokens(tokens), cursor(0), pq(ParsedQuery()) {
  rel_ref = {
      PqlTokenType::FOLLOWS,
      PqlTokenType::FOLLOWS_T,
      PqlTokenType::PARENT,
      PqlTokenType::PARENT_T,
      PqlTokenType::USES,
      PqlTokenType::MODIFIES,
      PqlTokenType::NEXT,
      PqlTokenType::NEXT_T,
      PqlTokenType::CALLS,
      PqlTokenType::CALLS_T,
      PqlTokenType::AFFECTS,
      PqlTokenType::AFFECTS_T
  };

  design_entities = {
      PqlTokenType::STMT,
      PqlTokenType::READ,
      PqlTokenType::PRINT,
      PqlTokenType::CALL,
      PqlTokenType::WHILE,
      PqlTokenType::IF,
      PqlTokenType::ASSIGN,
      PqlTokenType::VARIABLE,
      PqlTokenType::CONSTANT,
      PqlTokenType::PROCEDURE
  };

  expression_spec = {
      PqlTokenType::EXPR,
      PqlTokenType::SUB_EXPRESSION,
      PqlTokenType::UNDERSCORE,
      PqlTokenType::IDENT_WITH_QUOTES
  };

  ent_ref = {
      PqlTokenType::SYNONYM,
      PqlTokenType::UNDERSCORE,
      PqlTokenType::IDENT_WITH_QUOTES,
  };

  stmt_ref = {
      PqlTokenType::SYNONYM,
      PqlTokenType::UNDERSCORE,
      PqlTokenType::NUMBER,
  };

  stmt_ref_and_ent_ref = {
      PqlTokenType::SYNONYM,
      PqlTokenType::UNDERSCORE,
      PqlTokenType::NUMBER,
      PqlTokenType::IDENT_WITH_QUOTES,
  };

  allowed_synonyms = {
      PqlTokenType::SYNONYM,
      PqlTokenType::FOLLOWS,
      PqlTokenType::PARENT,
      PqlTokenType::USES,
      PqlTokenType::MODIFIES,
      PqlTokenType::NEXT,
      PqlTokenType::CALLS,
      PqlTokenType::AFFECTS,
      PqlTokenType::PATTERN,
      PqlTokenType::STMT,
      PqlTokenType::READ,
      PqlTokenType::PRINT,
      PqlTokenType::CALL,
      PqlTokenType::WHILE,
      PqlTokenType::IF,
      PqlTokenType::ASSIGN,
      PqlTokenType::VARIABLE,
      PqlTokenType::CONSTANT,
      PqlTokenType::PROCEDURE,
      PqlTokenType::SUCH,
      PqlTokenType::THAT,
      PqlTokenType::WITH,
      PqlTokenType::AND,
      PqlTokenType::SELECT,
      PqlTokenType::BOOLEAN
  };

  result_cl = {
      PqlTokenType::ATTRIBUTE,
      PqlTokenType::TUPLE,
      PqlTokenType::BOOLEAN,
      PqlTokenType::SYNONYM,
      PqlTokenType::FOLLOWS,
      PqlTokenType::PARENT,
      PqlTokenType::USES,
      PqlTokenType::MODIFIES,
      PqlTokenType::NEXT,
      PqlTokenType::CALLS,
      PqlTokenType::AFFECTS,
      PqlTokenType::PATTERN,
      PqlTokenType::STMT,
      PqlTokenType::READ,
      PqlTokenType::PRINT,
      PqlTokenType::CALL,
      PqlTokenType::WHILE,
      PqlTokenType::IF,
      PqlTokenType::ASSIGN,
      PqlTokenType::VARIABLE,
      PqlTokenType::CONSTANT,
      PqlTokenType::PROCEDURE,
      PqlTokenType::SUCH,
      PqlTokenType::THAT,
      PqlTokenType::WITH,
      PqlTokenType::AND,
      PqlTokenType::SELECT,
  };

  with_clause_ref = {
      PqlTokenType::IDENT_WITH_QUOTES,
      PqlTokenType::NUMBER,
      PqlTokenType::ATTRIBUTE,
  };

  rel_ref_arg_map = {
      {PqlTokenType::FOLLOWS, std::make_pair(stmt_ref, stmt_ref)},
      {PqlTokenType::FOLLOWS_T, std::make_pair(stmt_ref, stmt_ref)},
      {PqlTokenType::PARENT, std::make_pair(stmt_ref, stmt_ref)},
      {PqlTokenType::PARENT_T, std::make_pair(stmt_ref, stmt_ref)},
      {PqlTokenType::USES, std::make_pair(stmt_ref_and_ent_ref, ent_ref)},
      {PqlTokenType::MODIFIES, std::make_pair(stmt_ref_and_ent_ref, ent_ref)},
      {PqlTokenType::CALLS, std::make_pair(ent_ref, ent_ref)},
      {PqlTokenType::CALLS_T, std::make_pair(ent_ref, ent_ref)},
      {PqlTokenType::NEXT, std::make_pair(stmt_ref, stmt_ref)},
      {PqlTokenType::NEXT_T, std::make_pair(stmt_ref, stmt_ref)},
      {PqlTokenType::AFFECTS, std::make_pair(stmt_ref, stmt_ref)},
      {PqlTokenType::AFFECTS_T, std::make_pair(stmt_ref, stmt_ref)}
  };

  token_design_map = {
      {PqlTokenType::STMT, DesignEntityType::STMT},
      {PqlTokenType::ASSIGN, DesignEntityType::ASSIGN},
      {PqlTokenType::PRINT, DesignEntityType::PRINT},
      {PqlTokenType::PROCEDURE, DesignEntityType::PROCEDURE},
      {PqlTokenType::READ, DesignEntityType::READ},
      {PqlTokenType::CALL, DesignEntityType::CALL},
      {PqlTokenType::WHILE, DesignEntityType::WHILE},
      {PqlTokenType::IF, DesignEntityType::IF},
      {PqlTokenType::VARIABLE, DesignEntityType::VARIABLE},
      {PqlTokenType::CONSTANT, DesignEntityType::CONSTANT},
  };

  token_result_map = {
      {PqlTokenType::SYNONYM, ResultClauseType::SYNONYM},
      {PqlTokenType::ATTRIBUTE, ResultClauseType::ATTRIBUTE},
      {PqlTokenType::BOOLEAN, ResultClauseType::BOOLEAN},
      {PqlTokenType::TUPLE, ResultClauseType::TUPLE},
  };
}

std::string BOOLEAN_SYNONYM_VALUE = "BOOLEAN";

void PqlParser::MoveCursor(int movement) {
  cursor += movement;
}

PqlToken PqlParser::FetchToken() {
  if (cursor < tokens.size()) {
    return tokens[cursor];
  } else {
    return PqlToken(PqlTokenType::DUMMY, "");
  }
}

PqlToken PqlParser::ValidateToken(std::unordered_set<PqlTokenType> allowed_types) {
  PqlToken token = FetchToken();
  if (!allowed_types.count(token.type)) {
    throw InvalidQueryFormatException();
  }
  MoveCursor(1);
  return token;
}

PqlToken PqlParser::ValidateToken(PqlTokenType allowed_type) {
  PqlToken token = FetchToken();
  if (token.type != allowed_type) {
    throw InvalidQueryFormatException();
  }
  MoveCursor(1);
  return token;
}

void PqlParser::RevertToSynonymType() {
  if (allowed_synonyms.count(tokens[cursor].type)) {
    tokens[cursor].type = PqlTokenType::SYNONYM;
  }
}

int PqlParser::GetEndOfDeclarationCursor() {
  int last_semicolon = -1;
  for (int i = 0; i < tokens.size(); i++) {
    if (tokens[i].type == PqlTokenType::SEMICOLON) {
      last_semicolon = i;
    }
  }
  return last_semicolon;
}

void PqlParser::ParseDeclaration() {
  Declaration declarations = Declaration();
  int after_declaration_cursor = GetEndOfDeclarationCursor() + 1;
  if (after_declaration_cursor != 0) {
    while (cursor < after_declaration_cursor) {
      PqlToken design_entity_token = ValidateToken(design_entities);
      DesignEntityType design_entity_type = token_design_map[design_entity_token.type];
      ParseDeclarationVariables(design_entity_type, declarations);
    }
    pq.SetDeclarations(declarations);
  }
}

void PqlParser::ParseDeclarationVariables(DesignEntityType& design_entity_type, Declaration& declarations) {
  PqlToken next_token = FetchToken();
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
      ParseRelationshipClause();
    } else if (current_token.type == PqlTokenType::PATTERN) {
      ParsePatternClause();
    } else if (current_token.type == PqlTokenType::WITH) {
      ParseWithClause();
    } else {
      throw InvalidQueryFormatException();
    }
  }
}

void PqlParser::ParseResultClause() {
  PqlToken result_clause_token = ValidateToken(result_cl);
  std::unordered_map<std::string, DesignEntityType> declarations = pq.GetDeclaration().GetDeclarations();
  if (declarations.count(result_clause_token.value)) {
    result_clause_token.type = PqlTokenType::SYNONYM;
  }
  ResultClause result_clause = ResultClause(token_result_map[result_clause_token.type]);
  if (result_clause_token.type == PqlTokenType::TUPLE) {
    ParseTupleResultClause(result_clause, result_clause_token);
  } else if (result_clause_token.type == PqlTokenType::BOOLEAN) {
    ParseBooleanResultClause(result_clause, result_clause_token);
  } else if (result_clause_token.type == PqlTokenType::ATTRIBUTE) {
    ParseAttributeResultClause(result_clause, result_clause_token);
  } else {
    ParseSynonymResultClause(result_clause, result_clause_token);
  }
  pq.SetResultClause(result_clause);
}

void PqlParser::ParseSynonymResultClause(ResultClause& result_clause, PqlToken& token) {
  result_clause.AddValue(PqlToken(PqlTokenType::SYNONYM, token.value));
}

void PqlParser::ParseAttributeResultClause(ResultClause& result_clause, PqlToken& token) {
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

void PqlParser::ParseRelationshipClause() {
  PqlToken current_token = FetchToken();
  while (current_token.type == PqlTokenType::SUCH || current_token.type == PqlTokenType::AND) {
    if (current_token.type == PqlTokenType::SUCH) {
      ValidateToken(PqlTokenType::SUCH);
      ValidateToken(PqlTokenType::THAT);
    } else {
      MoveCursor(1);
    }
    PqlToken rel_ref_token = ValidateToken(rel_ref);
    ValidateToken(PqlTokenType::OPEN_PARENTHESIS);
    RevertToSynonymType();
    std::unordered_set<PqlTokenType> rel_ref_arg_type_first = rel_ref_arg_map[rel_ref_token.type].first;
    PqlToken first_arg = ValidateToken(rel_ref_arg_type_first);
    ValidateToken(PqlTokenType::COMMA);
    RevertToSynonymType();
    std::unordered_set<PqlTokenType> rel_ref_arg_type_second = rel_ref_arg_map[rel_ref_token.type].second;
    PqlToken second_arg = ValidateToken(rel_ref_arg_type_second);
    ValidateToken(PqlTokenType::CLOSED_PARENTHESIS);
    Relationship relationship = Relationship(rel_ref_token, first_arg, second_arg);
    pq.AddRelationship(relationship);
    current_token = FetchToken();
  }
}

void PqlParser::ParsePatternClause() {
  PqlToken current_token = FetchToken();
  while (current_token.type == PqlTokenType::PATTERN || current_token.type == PqlTokenType::AND) {
    MoveCursor(1);
    RevertToSynonymType();
    PqlToken pattern_syn = ValidateToken(allowed_synonyms);
    ValidateToken(PqlTokenType::OPEN_PARENTHESIS);
    RevertToSynonymType();
    PqlToken first_arg = ValidateToken(ent_ref);
    ValidateToken(PqlTokenType::COMMA);
    PqlToken second_arg = ValidateToken(expression_spec);
    PqlToken next_token = FetchToken();
    PqlToken third_arg;
    if (next_token.type == PqlTokenType::COMMA) {
      MoveCursor(1);
      third_arg = ValidateToken(PqlTokenType::UNDERSCORE);
      ValidateToken(PqlTokenType::CLOSED_PARENTHESIS);
    } else if (next_token.type == PqlTokenType::CLOSED_PARENTHESIS) {
      MoveCursor(1);
    } else {
      throw InvalidQueryFormatException();
    }
    current_token = FetchToken();
    Pattern pattern = Pattern(pattern_syn, first_arg, second_arg, third_arg);
    pq.AddPattern(pattern);
  }
}

void PqlParser::ParseWithClause() {
  PqlToken current_token = FetchToken();
  while (current_token.type == PqlTokenType::WITH || current_token.type == PqlTokenType::AND) {
    MoveCursor(1);
    PqlToken first_arg = ValidateToken(with_clause_ref);
    ValidateToken(PqlTokenType::EQUAL);
    PqlToken second_arg = ValidateToken(with_clause_ref);
    current_token = FetchToken();
    With with = With(first_arg, second_arg);
    pq.AddWithClause(with);
  }
}

ParsedQuery PqlParser::ParseQuery() {
  ParseDeclaration();
  ParseSelectClause();
  return pq;
}

















