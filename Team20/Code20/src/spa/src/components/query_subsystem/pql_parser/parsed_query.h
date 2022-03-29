#ifndef PARSER_PARSED_QUERY_H
#define PARSER_PARSED_QUERY_H

#include "../pql_lexer/pql_token.h"
#include "relationship.h"
#include "pattern.h"
#include "with_clause.h"
#include "result_clause.h"
#include "../utils.h"

#include <optional>

class ParsedQuery {
 private:
  ResultClause result_clause;
  std::unordered_map<std::string, DesignEntityType> declarations;
  std::vector<Relationship> relationships;
  std::vector<Pattern> patterns;
  std::vector<With> withs;

 public:
  ParsedQuery();
  void SetResultClause(ResultClause);
  void AddPattern(Pattern);
  void AddRelationship(Relationship);
  void AddWithClause(With);
  void SetDeclarations(std::unordered_map<std::string, DesignEntityType>);
  ResultClause GetResultClause();
  std::unordered_map<std::string, DesignEntityType> GetDeclaration();
  std::vector<Relationship> GetRelationships();
  std::vector<Pattern> GetPatterns();
  std::vector<With> GetWithClause();
};

extern std::unordered_map<PqlTokenType, DesignEntityType> token_design_map;
extern std::unordered_map<PqlTokenType, ResultClauseType> token_result_map;
#endif
