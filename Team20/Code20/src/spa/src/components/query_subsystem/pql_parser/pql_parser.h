#ifndef PQL_PARSER_H_
#define PQL_PARSER_H_

#include "parsed_query.h"
#include "../exceptions/invalid_query_format.h"

class PqlParser {
 private:
  std::vector<PqlToken> tokens;
  ParsedQuery pq = ParseQuery();
  int cursor;
  std::unordered_map<PqlTokenType, std::pair<std::unordered_set<PqlTokenType>, std::unordered_set<PqlTokenType>>>
      rel_ref_arg_map;
  void MoveCursor(int);
  PqlToken FetchToken();
  PqlToken FetchNextToken();
  PqlToken ValidateToken(std::unordered_set<PqlTokenType>);
  PqlToken ValidateToken(PqlTokenType);
  void RevertToSynonymType();
  int GetEndOfDeclarationCursor();
  void ParseDeclaration();
  void ParseDeclarationVariables(DesignEntityType &, Declaration &);
  void SetDuplicatedSynonyms(Declaration &, std::string);
  bool HasBooleanSynonym();
  void ParseSelectClause();
  void ParseResultClause();
  void ParseSynonymResultClause(ResultClause &, PqlToken &);
  void ParseAttributeResultClause(ResultClause &, PqlToken &);
  void ParseBooleanResultClause(ResultClause &, PqlToken &);
  void ParseTupleResultClause(ResultClause &, PqlToken &);
  void ParseRelationshipClause();
  void ParsePatternClause();
  void ParseWithClause();
 public:
  PqlParser(std::vector<PqlToken>);
  ParsedQuery ParseQuery();
};

#endif //PQL_PARSER_H_
