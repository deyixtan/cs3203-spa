#ifndef QUERY_VALIDATOR_H
#define QUERY_VALIDATOR_H

#include "components/query_subsystem/pql_lexer/pql_token.h"

class QueryValidator {
 private:
  std::vector<PqlToken> tokens_;
  bool IsValidSynonym(PqlToken synonym_token);
  bool IsValidRelRefToken(PqlToken rel_ref_token);
  bool IsValidDesignEntity(PqlToken design_entity_token);

  void ValidateRelRefClause(int rel_ref_clause_starting_index);
  void ValidateRelRefFirstArg(int first_arg_position);
  void ValidateRelRefSecondArg(int second_arg_position);
  int ValidatePatternArg(int pattern_arg_index);

  void ValidateDeclaration(int declaration_starting_index, int declaration_ended_index);
  void ValidateSelectClause(int select_clause_starting_index);
  void ValidateResultClause(int result_clause_starting_index);
  int ValidateSuchThatClause(int such_that_clause_starting_index);
  int ValidatePatternClause(int pattern_clause_starting_index);
  int ValidateWithClause(int with_clause_starting_index);

  void ReplaceSynonym(int synonym_index);

  int MINIMUM_TOKEN_TABLE_SIZE_FOR_SELECT_CLAUSE = 2;
  int MINIMUM_SELECT_CLAUSE_SIZE = 2;
  int MINIMUM_DECLARATION_SIZE = 3;
  int SUCH_THAT_WORD_COUNT = 2;
  int RELATIONSHIP_CLAUSE_OPEN_PARENTHESIS_POSITION = 1;
  int RELATIONSHIP_CLAUSE_FIRST_ARG_POSITION = 2;
  int RELATIONSHIP_CLAUSE_COMMA_POSITION = 3;
  int RELATIONSHIP_CLAUSE_SECOND_ARG_POSITION = 4;
  int RELATIONSHIP_CLAUSE_CLOSED_PARENTHESIS_POSITION = 5;
  int RELATIONSHIP_CLAUSE_SIZE = 6;
  int PATTERN_CLAUSE_SIZE_SHORT = 6;
  int PATTERN_CLAUSE_SIZE_LONG = 8;
  int PATTERN_CLAUSE_ARG_POSITION = 2;
  int PATTERN_CLAUSE_CLOSED_PARENTHESIS_POSITION_SHORT = 4;
  int PATTERN_CLAUSE_CLOSED_PARENTHESIS_POSITION_LONG = 6;
  int PATTERN_CLAUSE_FIRST_ARG_POSITION = 1;
  int PATTERN_CLAUSE_SECOND_ARG_POSITION = 3;
  int PATTERN_CLAUSE_THIRD_ARG_POSITION = 5;
  int PATTERN_CLAUSE_FIRST_COMMA_POSITION = 2;
  int PATTERN_CLAUSE_SECOND_COMMA_POSITION = 4;
  int WITH_CLAUSE_SIZE = 4;
  int WITH_CLAUSE_FIRST_ARG_POSITION = 1;
  int WITH_CLAUSE_SECOND_ARG_POSITION = 3;
  int WITH_CLAUSE_EQUAL_POSITION = 2;
 public:
  QueryValidator(std::vector<PqlToken>);
  std::vector<PqlToken> CheckValidation();
  int ValidateAssignWhilePattern(int pattern_arg_index);
  int ValidateIfPattern(int pattern_arg_index);
};

#endif
