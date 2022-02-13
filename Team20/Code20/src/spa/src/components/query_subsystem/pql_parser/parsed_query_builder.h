#ifndef SPA_SRC_SPA_SRC_COMPONENTS_QUERY_SUBSYSTEM_PQL_PARSER_PARSED_QUERY_BUILDER_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_QUERY_SUBSYSTEM_PQL_PARSER_PARSED_QUERY_BUILDER_H_

#include "./parsed_query.h"

class ParsedQueryBuilder {
 public:
  ParsedQuery build(std::vector<PqlToken>& tokens);

};

#endif //SPA_SRC_SPA_SRC_COMPONENTS_QUERY_SUBSYSTEM_PQL_PARSER_PARSED_QUERY_BUILDER_H_
