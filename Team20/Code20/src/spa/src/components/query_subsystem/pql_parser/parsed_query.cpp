#include "parsed_query.h"
#include <vector>

ParsedQuery::ParsedQuery() {
  std::vector<Declaration> vect;
  vect.push_back(Declaration());
  this->selected_synonym = PqlToken();
  this->declarations = vect;
  this->relationship = std::nullopt;
  this->pattern = std::nullopt;
};

void ParsedQuery::SetSynonym(PqlToken synonym) {
  selected_synonym = synonym;
}

void ParsedQuery::SetPattern(Pattern parsed_pattern) {
  pattern = std::make_optional(parsed_pattern);
}

void ParsedQuery::SetRelationship(Relationship parsed_relationship) {
  relationship = std::make_optional(parsed_relationship);
}

void ParsedQuery::SetDeclarations(std::vector<Declaration> parsed_declarations) {
  declarations = parsed_declarations;
}

PqlToken ParsedQuery::GetSynonym() {
  return selected_synonym;
}

std::optional<Relationship> ParsedQuery::GetRelationship() {
  return relationship;
}

std::optional<Pattern> ParsedQuery::GetPattern() {
  return pattern;
}

std::vector<Declaration> ParsedQuery::GetDeclaration() {
  return declarations;
}

ParsedQuery BuildParsedQuery(std::vector<PqlToken> &tokens) {
  ParsedQuery pq = ParsedQuery();
  std::vector<Declaration> declarations;
  PqlToken prev = tokens[0];
  int pos = 0;
  // Add declaration
  while(pos < tokens.size()) {
    PqlToken token = tokens[pos];
    if(token.type == PqlTokenType::SELECT) {
      PqlToken selected_synonym = tokens[++pos];
      pq.SetSynonym(selected_synonym);
      // skip such that tokens
      pos += 3;
    } else if(design_entities.count(token.type)) {
      PqlToken entity = token;
      pos++;
      while(tokens[pos].type != PqlTokenType::SEMICOLON) {
        // another synonym
        if(tokens[pos].type == PqlTokenType::SYNONYM) {
          Declaration decl = Declaration(tokens[pos], entity);
          declarations.push_back(decl);
          pos++;
          // skip commas
        } else {
          pos++;
        }
      }
      pq.SetDeclarations(declarations);
    } else if(rel_ref.count(token.type)) {
      PqlToken rlshp = token;
      // skip bracket
      pos += 2;
      PqlToken first = tokens[pos];
      pos += 2;
      PqlToken second = tokens[pos];
      Relationship relationship = Relationship(rlshp, first, second);
      pq.SetRelationship(relationship);
      pos += 2;
    } else if(token.type == PqlTokenType::PATTERN) {
      PqlToken syn = tokens[++pos];
      pos += 2;
      PqlToken first = tokens[pos];
      pos += 2;
      PqlToken second = tokens[pos];
      Pattern patt = Pattern(syn, first, second);
      pq.SetPattern(patt);
      pos++;
    } else {
      pos++;
    }
  }
  return pq;
}

