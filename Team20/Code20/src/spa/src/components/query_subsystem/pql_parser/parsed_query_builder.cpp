#include "parsed_query_builder.h"
#include "components/query_subsystem/pql_parser/query_grammar_error.h"
#include <unordered_map>

ParsedQuery ParsedQueryBuilder::Build(std::vector<PqlToken> &tokens) {
  ParsedQuery pq = ParsedQuery();
  std::unordered_map<std::string, DesignEntityType> declarations;
  PqlToken prev = tokens[0];
  int pos = 0;
  // Add declaration
  while(pos < tokens.size()) {
    PqlToken token = tokens[pos];
    if(token.type == PqlTokenType::SELECT) {
      PqlToken selected_synonym = tokens[++pos];
      if(selected_synonym.type != PqlTokenType::SYNONYM) {
        if(declarations.count(selected_synonym.value)) {
          selected_synonym = PqlToken(reverse_token_design_map[declarations[selected_synonym.value]], selected_synonym.value);
        }
      }
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
    } else if(token.type == PqlTokenType::WITH) {
      PqlToken attr = tokens[++pos];
      pos += 2;
      PqlToken comparator = tokens[pos];
      With with_clause = With(attr, comparator);
      pq.AddWithClause(with_clause);
    } else {
      pos++;
    }
  }
  return pq;
}