//
// Created by Yu Heem Lai on 13/2/22.
//

#include "parsed_query_builder.h"

ParsedQuery ParsedQueryBuilder::Build(std::vector<PqlToken> &tokens) {
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