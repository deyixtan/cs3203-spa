#include "parsed_query.h"
#include <vector>

ParsedQuery::ParsedQuery() {
  std::unordered_map<std::string, DesignEntityType> map;
  this->result_clause = ResultClause();
  this->declarations = map;
};

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

void ParsedQuery::SetDeclarations(std::unordered_map<std::string, DesignEntityType> parsed_declarations) {
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

std::unordered_map<std::string, DesignEntityType> ParsedQuery::GetDeclaration() {
  return declarations;
}

std::vector<With> ParsedQuery::GetWithClause() {
  return withs;
}

std::unordered_map<PqlTokenType, DesignEntityType> token_design_map {
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

std::unordered_map<DesignEntityType, PqlTokenType> reverse_token_design_map {
    {DesignEntityType::STMT, PqlTokenType::STMT},
    {DesignEntityType::ASSIGN, PqlTokenType::ASSIGN},
    {DesignEntityType::PRINT, PqlTokenType::PRINT},
    {DesignEntityType::PROCEDURE, PqlTokenType::PROCEDURE},
    {DesignEntityType::READ, PqlTokenType::READ},
    {DesignEntityType::CALL, PqlTokenType::CALL},
    {DesignEntityType::WHILE, PqlTokenType::WHILE},
    {DesignEntityType::IF, PqlTokenType::IF},
    {DesignEntityType::VARIABLE, PqlTokenType::VARIABLE},
    {DesignEntityType::CONSTANT, PqlTokenType::CONSTANT},
};
