#include "parsed_query.h"
#include <vector>

ParsedQuery::ParsedQuery() {
  std::vector<Declaration> vect;
  vect.push_back(Declaration());
  this->selected_synonym = PqlToken();
  this->declarations = vect;
};

void ParsedQuery::SetSynonym(PqlToken synonym) {
  selected_synonym = synonym;
}

void ParsedQuery::AddPattern(Pattern parsed_pattern) {
  patterns.push_back(parsed_pattern);
}

void ParsedQuery::AddRelationship(Relationship parsed_relationship) {
  relationships.push_back(parsed_relationship);
}

void ParsedQuery::SetDeclarations(std::vector<Declaration> parsed_declarations) {
  declarations = parsed_declarations;
}

PqlToken ParsedQuery::GetSynonym() {
  return selected_synonym;
}

std::vector<Relationship> ParsedQuery::GetRelationships() {
  return relationships;
}

std::vector<Pattern> ParsedQuery::GetPatterns() {
  return patterns;
}

std::vector<Declaration> ParsedQuery::GetDeclaration() {
  return declarations;
}

ParsedQuery ParsedQuery::BuildParsedQuery(std::vector<PqlToken> &tokens) {
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
      pq.AddRelationship(relationship);
      pos += 2;
    } else if(token.type == PqlTokenType::PATTERN) {
      PqlToken syn = tokens[++pos];
      pos += 2;
      PqlToken first = tokens[pos];
      pos += 2;
      PqlToken second = tokens[pos];
      Pattern patt = Pattern(syn, first, second);
      pq.AddPattern(patt);
      pos++;
    } else {
      pos++;
    }
  }
  return pq;
}

