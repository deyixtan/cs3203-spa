#include "parsed_query.h"
#include <vector>

ParsedQuery::ParsedQuery() {
  result_clause = ResultClause();
  declarations = Declaration();
  relationships = std::vector<Relationship>();
  patterns = std::vector<Pattern>();
  withs = std::vector<With>();
}

void ParsedQuery::SetResultClause(ResultClause parsed_result_clause) {
  result_clause = parsed_result_clause;
}

void ParsedQuery::AddPattern(Pattern parsed_pattern) {
  patterns.push_back(parsed_pattern);
}

void ParsedQuery::AddRelationship(Relationship parsed_relationship) {
  relationships.push_back(parsed_relationship);
}

void ParsedQuery::AddWithClause(With parsed_with_clause) {
  withs.push_back(parsed_with_clause);
}

void ParsedQuery::SetDeclarations(Declaration &parsed_declarations) {
  declarations = parsed_declarations;
}

ResultClause ParsedQuery::GetResultClause() {
  return result_clause;
}

std::vector<Relationship> ParsedQuery::GetRelationships() {
  return relationships;
}

std::vector<Pattern> ParsedQuery::GetPatterns() {
  return patterns;
}

Declaration ParsedQuery::GetDeclaration() {
  return declarations;
}

std::vector<With> ParsedQuery::GetWithClause() {
  return withs;
}

