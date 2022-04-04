#ifndef SPA_SRC_SPA_SRC_COMPONENTS_QUERY_SUBSYSTEM_PQL_PARSER_WITH_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_QUERY_SUBSYSTEM_PQL_PARSER_WITH_H_

#include "../pql_lexer/pql_token.h"

class With {
 private:
  PqlToken first_;
  PqlToken second_;

 public:
  With();
  With(PqlToken, PqlToken);
  PqlToken GetFirst();
  PqlToken GetSecond();
};

#endif //SPA_SRC_SPA_SRC_COMPONENTS_QUERY_SUBSYSTEM_PQL_PARSER_WITH_H_
