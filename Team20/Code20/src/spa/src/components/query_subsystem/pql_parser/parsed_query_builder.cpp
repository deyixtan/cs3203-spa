#include "parsed_query_builder.h"
#include "components/query_subsystem/pql_parser/query_grammar_error.h"
#include <unordered_map>

ResultClause ParsedQueryBuilder::BuildResultClause(PqlToken& result_clause_token) {
  ResultClause result_clause;
  std::vector<PqlToken> values;
  if (result_clause_token.type == PqlTokenType::SYNONYM) {
    values.push_back(result_clause_token);
    result_clause = ResultClause(ResultClauseType::SYNONYM, values);
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

ParsedQuery ParsedQueryBuilder::Build(std::vector<PqlToken> &tokens) {
  ParsedQuery pq = ParsedQuery();
  std::unordered_map<std::string, DesignEntityType> declarations;
  PqlTokenType prev = tokens[0].type;
  int pos = 0;
  // Add declaration
  while(pos < tokens.size()) {
    PqlToken token = tokens[pos];
    if(token.type == PqlTokenType::SELECT) {
      PqlToken result_clause_token = tokens[++pos];
      if(result_clause_token.type == PqlTokenType::BOOLEAN) {
        if(declarations.count(result_clause_token.value)) {
          result_clause_token.type = PqlTokenType::SYNONYM;
        }
      }
      ResultClause result_clause = BuildResultClause(result_clause_token);
      pq.SetResultClause(result_clause);
      // skip such that tokens
      pos += 1;
    } else if (token.type == PqlTokenType::SUCH) {
      // skip the token following such
      pos += 2;
    } else if(design_entities.count(token.type)) {
      PqlToken entity = token;
      pos++;
      while(tokens[pos].type != PqlTokenType::SEMICOLON) {
        // another synonym
        if(tokens[pos].type == PqlTokenType::SYNONYM) {
          if(declarations.count(tokens[pos].value)) {
            throw DUPLICATE_DECLARATION_SYNONYM;
          } else {
            DesignEntityType design_entity = token_design_map[entity.type];
            declarations.insert({ tokens[pos].value, design_entity });
          }
          pos++;
          // skip commas
        } else {
          pos++;
        }
      }
      pq.SetDeclarations(declarations);
    } else if(rel_ref.count(token.type)) {
      PqlToken rlshp = token;
      prev = token.type;
      // skip bracket
      pos += 2;
      PqlToken first = tokens[pos];
      pos += 2;
      PqlToken second = tokens[pos];
      Relationship relationship = Relationship(rlshp, first, second);
      pq.AddRelationship(relationship);
      pos += 2;
    } else if(token.type == PqlTokenType::PATTERN) {
      prev = token.type;
      PqlToken syn = tokens[++pos];
      pos += 2;
      PqlToken first = tokens[pos];
      pos += 2;
      PqlToken second = tokens[pos];
      Pattern patt;
      if(pos + 1 < tokens.size() && tokens[pos + 1].value == ",") {
        pos += 2;
        PqlToken third = tokens[pos];
        patt = Pattern(syn, first, second, third);
      } else {
        patt = Pattern(syn, first, second);
      }
      pq.AddPattern(patt);
      pos++;
    } else if(token.type == PqlTokenType::WITH) {
      PqlToken attr = tokens[++pos];
      prev = attr.type;
      pos += 2;
      PqlToken comparator = tokens[pos];
      With with_clause = With(attr, comparator);
      pq.AddWithClause(with_clause);
    } else if(token.type == PqlTokenType::AND && tokens[pos + 1].type == PqlTokenType::SYNONYM) {
      if(prev == PqlTokenType::PATTERN){
        PqlToken dummy_token = PqlToken(PqlTokenType::PATTERN, "pattern");
        auto itPos = tokens.begin() + pos + 1;
        tokens.insert(itPos, dummy_token);
      } else {
        throw INVALID_AND_CLAUSE_FORMAT;
      }
    } else {
      pos++;
    }
  }
  return pq;
}