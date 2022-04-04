#include "parsed_query.h"
#include <vector>

ParsedQuery::ParsedQuery() : declarations(declarations),
                             result_clause(result_clause),
                             relationships(relationships),
                             patterns(patterns),
                             withs(withs) {}

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

void ParsedQuery::SetDeclarations(Declaration parsed_declarations) {
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

std::unordered_map<PqlTokenType, ResultClauseType> token_result_map {
    {PqlTokenType::SYNONYM, ResultClauseType::SYNONYM},
    {PqlTokenType::ATTRIBUTE, ResultClauseType::ATTRIBUTE},
    {PqlTokenType::BOOLEAN, ResultClauseType::BOOLEAN},
    {PqlTokenType::TUPLE, ResultClauseType::TUPLE},
};
