#include "parsed_query.h"
#include <vector>

ParsedQuery::ParsedQuery() {
  std::unordered_map<std::string, DesignEntityType> map;
  this->selected_synonym = PqlToken();
  this->declarations = map;
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

void ParsedQuery::SetDeclarations(std::unordered_map<std::string, DesignEntityType> parsed_declarations) {
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

std::unordered_map<std::string, DesignEntityType> ParsedQuery::GetDeclaration() {
  return declarations;
}
