#include "parsed_query_builder.h"
#include "components/query_subsystem/pql_parser/query_grammar_error.h"
#include <unordered_map>

ParsedQueryBuilder::ParsedQueryBuilder(std::vector<PqlToken> tokens) : tokens_(tokens){}

ResultClause ParsedQueryBuilder::BuildResultClause(PqlToken& result_clause_token) {
  ResultClause result_clause;
  std::vector<PqlToken> values;
  if (result_clause_token.type == PqlTokenType::SYNONYM) {
    values.push_back(result_clause_token);
    result_clause = ResultClause(ResultClauseType::SYNONYM, values);
  } else if (result_clause_token.type == PqlTokenType::ATTRIBUTE) {
    values.push_back(result_clause_token);
    result_clause = ResultClause(ResultClauseType::ATTRIBUTE, values);
  } else if (result_clause_token.type == PqlTokenType::BOOLEAN) {
    values.push_back(result_clause_token);
    result_clause = ResultClause(ResultClauseType::BOOLEAN, values);
  } else if (result_clause_token.type == PqlTokenType::TUPLE) {
    SplitTuple(result_clause_token, values);
    result_clause = ResultClause(ResultClauseType::TUPLE, values);
  } else {
    throw INVALID_SELECT_CLAUSE_FORMAT;
  }

  return result_clause;
}

void ParsedQueryBuilder::SplitTuple(PqlToken& tuple, std::vector<PqlToken>& values) {
  std::string tuple_string = tuple.value.substr(1, tuple.value.length() - 2);
  std::string temp_string;
  for (auto c : tuple_string) {
    if (c == ',') {
      temp_string = Utils::RemoveSpace(temp_string);
      if (Utils::ContainsDot(temp_string)) {
        values.push_back(PqlToken(PqlTokenType::ATTRIBUTE, temp_string));
      } else {
        values.push_back(PqlToken(PqlTokenType::SYNONYM, temp_string));
      }
      temp_string.clear();
    } else {
      temp_string += c;
    }
  }
  if (Utils::ContainsDot(temp_string)) {
    values.push_back(PqlToken(PqlTokenType::ATTRIBUTE, temp_string));
  } else {
    values.push_back(PqlToken(PqlTokenType::SYNONYM, temp_string));
  }
}

ParsedQuery ParsedQueryBuilder::Build() {
  ParsedQuery pq = ParsedQuery();
  std::unordered_map<std::string, DesignEntityType> declarations;
  PqlTokenType prev = tokens_[0].type;
  int pos = 0;
  // Add declaration
  while(pos < tokens_.size()) {
    PqlToken token = tokens_[pos];
    if(token.type == PqlTokenType::SELECT) {
      pos = ParseResultClause(pq, declarations, pos);
    } else if (token.type == PqlTokenType::SUCH) {
      // skip the token following such
      tokens_.erase(tokens_.begin(), tokens_.begin() + 2);
    } else if(design_entities.count(token.type)) {
      pos = ParseDeclarations(pq, declarations, pos);
    } else if(rel_ref.count(token.type)) {
      PqlToken rlshp = token;
      prev = token.type;
      pos = ParseRelationship(pq, pos);
    } else if(token.type == PqlTokenType::PATTERN) {
      prev = token.type;
      pos = ParsePattern(pq, pos);
    } else if(token.type == PqlTokenType::ATTRIBUTE) {
      prev = token.type;
      pos = ParseWithClause(pq, pos);
    } else if(token.type == PqlTokenType::AND && tokens_[pos + 1].type == PqlTokenType::SYNONYM) {
      if (prev == PqlTokenType::PATTERN) {
        PqlToken dummy_token = PqlToken(PqlTokenType::PATTERN, "pattern");
        auto itPos = tokens_.begin() + pos + 1;
        tokens_.insert(itPos, dummy_token);
      } else {
        throw INVALID_AND_CLAUSE_FORMAT;
      }
    } else {
      pos++;
    }
  }
  return pq;
}

int ParsedQueryBuilder::ParseResultClause(ParsedQuery &pq, std::unordered_map<std::string, DesignEntityType> &declarations, int pos) {
  PqlToken result_clause_token = tokens_[++pos];
  if(result_clause_token.type == PqlTokenType::BOOLEAN) {
    if(declarations.count(result_clause_token.value)) {
      result_clause_token = PqlToken(PqlTokenType::SYNONYM, result_clause_token.value);
    }
  }
  ResultClause result_clause = BuildResultClause(result_clause_token);
  pq.SetResultClause(result_clause);
  // skip such that tokens
  pos += 1;
  return pos;
}


int ParsedQueryBuilder::ParseDeclarations(ParsedQuery &pq, std::unordered_map<std::string, DesignEntityType> &declarations, int pos) {
  PqlToken entity = tokens_[pos];
  pos++;
  while(tokens_[pos].type != PqlTokenType::SEMICOLON) {
    // another synonym
    if(tokens_[pos].type == PqlTokenType::SYNONYM || tokens_[pos].type == PqlTokenType::BOOLEAN) {
      if(declarations.count(tokens_[pos].value)) {
        throw DUPLICATE_DECLARATION_SYNONYM;
      } else {
        DesignEntityType design_entity = token_design_map[entity.type];
        declarations.insert({ tokens_[pos].value, design_entity });
      }
      pos++;
      // skip commas
    } else {
      pos++;
    }
  }
  pq.SetDeclarations(declarations);
  return pos;
}

int ParsedQueryBuilder::ParsePattern(ParsedQuery &pq, int pos) {
  PqlToken syn = tokens_[++pos];
  pos += 2;
  PqlToken first = tokens_[pos];
  pos += 2;
  PqlToken second = tokens_[pos];
  Pattern patt;
  if(pos + 1 < tokens_.size() && tokens_[pos + 1].value == ",") {
    pos += 2;
    PqlToken third = tokens_[pos];
    patt = Pattern(syn, first, second, third);
  } else {
    patt = Pattern(syn, first, second);
  }
  pq.AddPattern(patt);
  return pos;
}

int ParsedQueryBuilder::ParseRelationship(ParsedQuery &pq, int pos) {
  PqlToken rlshp = tokens_[pos];
  pos += 2;
  // skip bracket
  PqlToken first = tokens_[pos];
  pos += 2;
  PqlToken second = tokens_[pos];
  Relationship relationship = Relationship(rlshp, first, second);
  pq.AddRelationship(relationship);
  pos += 2;
  return pos;
}

int ParsedQueryBuilder::ParseWithClause(ParsedQuery &pq, int pos) {
  PqlToken attr = tokens_[pos];
  pos += 2;
  PqlToken comparator = tokens_[pos];
  With with_clause = With(attr, comparator);
  pq.AddWithClause(with_clause);
  pos += 1;
  return pos;
}
