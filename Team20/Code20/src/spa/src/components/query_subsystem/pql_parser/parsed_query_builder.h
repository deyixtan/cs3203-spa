#ifndef SPA_SRC_SPA_SRC_COMPONENTS_QUERY_SUBSYSTEM_PQL_PARSER_PARSED_QUERY_BUILDER_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_QUERY_SUBSYSTEM_PQL_PARSER_PARSED_QUERY_BUILDER_H_

#include "./parsed_query.h"
#include "../utils.h"

class ParsedQueryBuilder {
 public:
  ParsedQueryBuilder(std::vector<PqlToken> tokens);
  ParsedQuery Build();
 private:
  std::vector<PqlToken> tokens_;
  ResultClause BuildResultClause(PqlToken& result_clause_token);
  void SplitTuple(PqlToken& tuple, std::vector<PqlToken>& values);
  int ParseResultClause(ParsedQuery &pq, std::unordered_map<std::string, DesignEntityType> &declarations, int pos);
  int ParseDeclarations(ParsedQuery &pq, std::unordered_map<std::string, DesignEntityType> &declarations, int pos);
  int ParsePattern(ParsedQuery &pq, std::unordered_map<std::string, DesignEntityType> &declarations, int pos);
  int ParseRelationship(ParsedQuery &pq, int pos);
  int ParseWithClause(ParsedQuery &pq, int pos);
};

#endif //SPA_SRC_SPA_SRC_COMPONENTS_QUERY_SUBSYSTEM_PQL_PARSER_PARSED_QUERY_BUILDER_H_
