#ifndef SPA_SRC_SPA_SRC_COMPONENTS_QUERY_SUBSYSTEM_PQL_PARSER_PARSED_QUERY_BUILDER_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_QUERY_SUBSYSTEM_PQL_PARSER_PARSED_QUERY_BUILDER_H_

#include "./parsed_query.h"
#include "../utils.h"

class ParsedQueryBuilder {
 public:
  ParsedQuery Build(std::vector<PqlToken>& tokens);
  void SplitTuple(PqlToken& tuple, std::vector<PqlToken>& values);
 private:
  ResultClause BuildResultClause(PqlToken& result_clause_token);
  //void SplitTuple(PqlToken& tuple, std::vector<PqlToken>& values);
};

#endif //SPA_SRC_SPA_SRC_COMPONENTS_QUERY_SUBSYSTEM_PQL_PARSER_PARSED_QUERY_BUILDER_H_
