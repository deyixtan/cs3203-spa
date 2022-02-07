#ifndef QUERY_VALIDATOR_H
#define QUERY_VALIDATOR_H

#include "../pql_lexer/pql_token.h"

class QueryValidator {
 private:
  std::vector<PqlToken> tokens_;
  bool IsValidSynonym(PqlToken synonym_token);
  bool IsValidRelRefToken(PqlToken rel_ref_token);
  bool IsValidDesignEntity(PqlToken design_entity_token);
  bool IsValidRelRefClause(std::vector<PqlToken> rel_ref_tokens);
  void IsValidRelRefArg(std::vector<PqlToken> rel_ref_tokens, PqlTokenType type);
  void IsValidDeclaration(std::vector<PqlToken> declaration_query);
  void IsValidPatternClause(std::vector<PqlToken> pattern_clause);
  void IsValidSelectClause(std::vector<PqlToken> select_clause);
 public:
  QueryValidator(std::vector<PqlToken>);
  std::vector<PqlToken> CheckValidation();
};

#endif
